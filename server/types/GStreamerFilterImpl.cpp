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

#include "GStreamerFilterImpl.hpp"
#include <generated/MediaPipeline.hpp>

#define GST_CAT_DEFAULT kurento_gstreamer_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoGStreamerFilterImpl"


namespace kurento
{

GStreamerFilterImpl::GStreamerFilterImpl (
  const std::string &command,
  std::shared_ptr< MediaObjectImpl > parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  // TODO:
}

std::shared_ptr<MediaObject>
GStreamerFilter::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline,
  const std::string &command, int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new GStreamerFilterImpl (command,
                                       std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                                       garbagePeriod) );

  return object;
}

GStreamerFilterImpl::StaticConstructor GStreamerFilterImpl::staticConstructor;

GStreamerFilterImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
