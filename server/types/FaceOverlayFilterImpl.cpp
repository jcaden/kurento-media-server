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

#include "FaceOverlayFilterImpl.hpp"
#include <generated/MediaPipeline.hpp>

#define GST_CAT_DEFAULT kurento_face_overlay_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoFaceOverlayFilterImpl"


namespace kurento
{

FaceOverlayFilterImpl::FaceOverlayFilterImpl (
  std::shared_ptr< MediaObjectImpl > parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  // TODO:
}

void
FaceOverlayFilterImpl::unsetOverlayedImage ()
{
  // TODO:
}

void
FaceOverlayFilterImpl::setOverlayedImage (const std::string &uri,
    float offsetXPercent, float offsetYPercent, float widthPercent,
    float heightPercent)
{
  // TODO:
}

std::shared_ptr<MediaObject>
FaceOverlayFilter::Factory::createObject (std::shared_ptr<MediaPipeline>
    mediaPipeline,
    int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new FaceOverlayFilterImpl (
                                         std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                                         garbagePeriod) );

  return object;
}

FaceOverlayFilterImpl::StaticConstructor
FaceOverlayFilterImpl::staticConstructor;

FaceOverlayFilterImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
