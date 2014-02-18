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

#include "HttpGetEndpointImpl.hpp"
#include <generated/MediaPipeline.hpp>
#include <generated/MediaProfileSpecType.hpp>

namespace kurento
{
HttpGetEndpointImpl::HttpGetEndpointImpl (
  bool terminateOnEOS, std::shared_ptr<MediaProfileSpecType> mediaProfile,
  int disconnectionTimeout, std::shared_ptr< MediaObjectImpl > mediaPipeline,
  int garbagePeriod) :
  HttpEndpointImpl (disconnectionTimeout, mediaPipeline, garbagePeriod)
{

}

std::shared_ptr< MediaObject >
HttpGetEndpoint::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline, bool terminateOnEOS,
  std::shared_ptr<MediaProfileSpecType> mediaProfile, int disconnectionTimeout,
  int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new HttpGetEndpointImpl (
                                         terminateOnEOS, mediaProfile, disconnectionTimeout,
                                         std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                                         garbagePeriod) );

  return object;
}

} /* kurento */
