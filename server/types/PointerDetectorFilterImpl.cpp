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

#include "PointerDetectorFilterImpl.hpp"
#include <generated/MediaPipeline.hpp>
#include <generated/PointerDetectorWindowMediaParam.hpp>

#define GST_CAT_DEFAULT kurento_pointer_detector_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoPointerDetectorFilterImpl"


namespace kurento
{

PointerDetectorFilterImpl::PointerDetectorFilterImpl (
  const std::vector<std::shared_ptr<PointerDetectorWindowMediaParam>> &windows,
  std::shared_ptr< MediaObjectImpl > parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  // TODO:
}

void
PointerDetectorFilterImpl::addWindow (
  std::shared_ptr<PointerDetectorWindowMediaParam> window)
{
  // TODO:
}

void
PointerDetectorFilterImpl::clearWindows ()
{
  // TODO:
}

void
PointerDetectorFilterImpl::removeWindow (const std::string &windowId)
{
  // TODO:
}

std::shared_ptr<MediaObject>
PointerDetectorFilter::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline,
  const std::vector<std::shared_ptr<PointerDetectorWindowMediaParam>> &windows,
  int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new PointerDetectorFilterImpl (
                                         windows, std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                                         garbagePeriod) );

  return object;
}

PointerDetectorFilterImpl::StaticConstructor
PointerDetectorFilterImpl::staticConstructor;

PointerDetectorFilterImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
