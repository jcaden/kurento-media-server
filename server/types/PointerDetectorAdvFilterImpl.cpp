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

#define GST_CAT_DEFAULT kurento_pointer_detector_adv_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoPointerDetectorAdvFilterImpl"


namespace kurento
{

PointerDetectorAdvFilterImpl::PointerDetectorAdvFilterImpl (
  std::shared_ptr<WindowParam> calibrationRegion,
  std::shared_ptr<PointerDetectorWindowMediaParam> window,
  std::shared_ptr<MediaObjectImpl> parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  // TODO:
}

void
PointerDetectorAdvFilterImpl::addWindow (
  std::shared_ptr<PointerDetectorWindowMediaParam> window)
{
  // TODO:
}

void
PointerDetectorAdvFilterImpl::clearWindows ()
{
  // TODO:
}

void
PointerDetectorAdvFilterImpl::trackcolourFromCalibrationRegion ()
{
  // TODO:
}

void
PointerDetectorAdvFilterImpl::removeWindow (const std::string &windowId)
{
  // TODO:
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
