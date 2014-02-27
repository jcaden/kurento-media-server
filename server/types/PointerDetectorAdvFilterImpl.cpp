/*
 * (C) Copyright 2014 Kurento (http://kurento.org/)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "PointerDetectorAdvFilterImpl.hpp"
#include <generated/MediaPipeline.hpp>
#include <generated/WindowParam.hpp>
#include <generated/PointerDetectorWindowMediaParam.hpp>
#include "MediaPipelineImpl.hpp"
#include <KurentoException.hpp>

#define GST_CAT_DEFAULT kurento_pointer_detector_adv_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoPointerDetectorAdvFilterImpl"

#define WINDOWS_LAYOUT "windows-layout"
#define CALIBRATION_AREA "calibration-area"
#define CALIBRATE_COLOR "calibrate-color"

namespace kurento
{

static void
bus_message_adaptor (GstBus *bus, GstMessage *message, gpointer data)
{
  auto func = reinterpret_cast<std::function<void (GstMessage *message) >*>
              (data);

  (*func) (message);
}

PointerDetectorAdvFilterImpl::PointerDetectorAdvFilterImpl (
  std::shared_ptr<WindowParam> calibrationRegion,
  std::shared_ptr<PointerDetectorWindowMediaParam> window,
  std::shared_ptr<MediaObjectImpl> parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  GstBus *bus;
  std::shared_ptr<MediaPipelineImpl> pipe;
  GstStructure *calibrationArea;

  pipe = std::dynamic_pointer_cast<MediaPipelineImpl> (getMediaPipeline() );

  g_object_set (element, "filter-factory", "pointerdetector", NULL);

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipe->getPipeline() ) );

  g_object_get (G_OBJECT (element), "filter", &pointerDetector, NULL);

  if (pointerDetector == NULL) {
    g_object_unref (bus);
    throw KurentoException ("Media Object not available");
  }

  calibrationArea = gst_structure_new (
                      "calibration_area",
                      "x", G_TYPE_INT, calibrationRegion->getTopRightCornerX(),
                      "y", G_TYPE_INT, calibrationRegion->getTopRightCornerY(),
                      "width", G_TYPE_INT, calibrationRegion->getWidth(),
                      "height", G_TYPE_INT, calibrationRegion->getHeight(),
                      NULL);
  g_object_set (G_OBJECT (pointerDetector), CALIBRATION_AREA,
                calibrationArea, NULL);
  gst_structure_free (calibrationArea);

  // TODO: Window showld be an array
  if (window) {
    addWindow (window);
  }

  busMessageLambda = [&] (GstMessage * message) {
    const GstStructure *st;
    gchar *windowID;
    const gchar *type;
    std::string windowIDStr, typeStr;

    if (GST_MESSAGE_SRC (message) != GST_OBJECT (pointerDetector) ||
        GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT) {
      return;
    }

    st = gst_message_get_structure (message);
    type = gst_structure_get_name (st);

    if ( (g_strcmp0 (type, "window-out") != 0) &&
         (g_strcmp0 (type, "window-in") != 0) ) {
      GST_WARNING ("The message does not have the correct name");
      return;
    }

    if (!gst_structure_get (st, "window", G_TYPE_STRING , &windowID, NULL) ) {
      GST_WARNING ("The message does not contain the window ID");
      return;
    }

    windowIDStr = windowID;
    typeStr = type;

    g_free (windowID);

    if (typeStr == "window-in") {
      WindowIn event (windowIDStr, shared_from_this(), WindowIn::getName() );

      signalWindowIn (event);
    } else if (typeStr == "window-out") {
      WindowOut event (windowIDStr, shared_from_this(), WindowOut::getName() );

      signalWindowOut (event);
    }
  };

  bus_handler_id = g_signal_connect (bus, "message",
                                     G_CALLBACK (bus_message_adaptor),
                                     &busMessageLambda);
  g_object_unref (bus);
  // There is no need to reference pointerdetector because its life cycle is the same as the filter life cycle
  g_object_unref (pointerDetector);
}

PointerDetectorAdvFilterImpl::~PointerDetectorAdvFilterImpl()
{
  std::shared_ptr<MediaPipelineImpl> pipe;

  pipe = std::dynamic_pointer_cast<MediaPipelineImpl> (getMediaPipeline() );
  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipe->getPipeline() ) );
  g_signal_handler_disconnect (bus, bus_handler_id);
  g_object_unref (bus);
}

void
PointerDetectorAdvFilterImpl::addWindow (
  std::shared_ptr<PointerDetectorWindowMediaParam> window)
{
  GstStructure *buttonsLayout, *buttonsLayoutAux;

  buttonsLayoutAux = gst_structure_new (
                       window->getId().c_str(),
                       "upRightCornerX", G_TYPE_INT, window->getUpperRightX(),
                       "upRightCornerY", G_TYPE_INT, window->getUpperRightY(),
                       "width", G_TYPE_INT, window->getWidth(),
                       "height", G_TYPE_INT, window->getHeight(),
                       "id", G_TYPE_STRING, window->getId().c_str(),
                       NULL);

  if (window->isSetInactiveImage() ) {
    gst_structure_set (buttonsLayoutAux, "inactive_uri",
                       G_TYPE_STRING, window->getInactiveImage().c_str(), NULL);
  }

  if (window->isSetImageTransparency() ) {
    gst_structure_set (buttonsLayoutAux, "transparency",
                       G_TYPE_DOUBLE, double (window->getImageTransparency() ), NULL);
  }

  if (window->isSetImage() ) {
    gst_structure_set (buttonsLayoutAux, "active_uri",
                       G_TYPE_STRING, window->getImage().c_str(), NULL);
  }


  /* The function obtains the actual window list */
  g_object_get (G_OBJECT (pointerDetector), WINDOWS_LAYOUT, &buttonsLayout,
                NULL);
  gst_structure_set (buttonsLayout,
                     window->getId().c_str(), GST_TYPE_STRUCTURE,
                     buttonsLayoutAux, NULL);

  g_object_set (G_OBJECT (pointerDetector), WINDOWS_LAYOUT, buttonsLayout, NULL);

  gst_structure_free (buttonsLayout);
  gst_structure_free (buttonsLayoutAux);
}

void
PointerDetectorAdvFilterImpl::clearWindows ()
{
  GstStructure *buttonsLayout;

  buttonsLayout = gst_structure_new_empty  ("buttonsLayout");
  g_object_set (G_OBJECT (pointerDetector), WINDOWS_LAYOUT, buttonsLayout,
                NULL);
  gst_structure_free (buttonsLayout);
}

void
PointerDetectorAdvFilterImpl::trackcolourFromCalibrationRegion ()
{
  g_signal_emit_by_name (pointerDetector, CALIBRATE_COLOR, NULL);
}

void
PointerDetectorAdvFilterImpl::removeWindow (const std::string &windowId)
{
  GstStructure *buttonsLayout;
  gint len;

  /* The function obtains the actual window list */
  g_object_get (G_OBJECT (pointerDetector), WINDOWS_LAYOUT, &buttonsLayout,
                NULL);
  len = gst_structure_n_fields (buttonsLayout);

  if (len == 0) {
    GST_WARNING ("There are no windows in the layout");
    return;
  }

  for (int i = 0; i < len; i++) {
    const gchar *name;
    name = gst_structure_nth_field_name (buttonsLayout, i);

    if (windowId == name) {
      /* this window will be removed */
      gst_structure_remove_field (buttonsLayout, name);
    }
  }

  /* Set the buttons layout list without the window with id = id */
  g_object_set (G_OBJECT (pointerDetector), WINDOWS_LAYOUT, buttonsLayout, NULL);

  gst_structure_free (buttonsLayout);
}

std::shared_ptr<MediaObject>
PointerDetectorAdvFilter::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline,
  std::shared_ptr<WindowParam> calibrationRegion,
  std::shared_ptr<PointerDetectorWindowMediaParam> window, int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new PointerDetectorAdvFilterImpl (
                                         calibrationRegion, window,
                                         std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                                         garbagePeriod) );

  return object;
}

PointerDetectorAdvFilterImpl::StaticConstructor
PointerDetectorAdvFilterImpl::staticConstructor;

PointerDetectorAdvFilterImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
