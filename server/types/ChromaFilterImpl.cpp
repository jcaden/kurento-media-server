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

#include "ChromaFilterImpl.hpp"
#include <generated/MediaPipeline.hpp>

#define GST_CAT_DEFAULT kurento_chroma_filter_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoChromaFilterImpl"


namespace kurento
{

ChromaFilterImpl::ChromaFilterImpl (
  std::shared_ptr<WindowParam> window, const std::string &backgroundImage,
  std::shared_ptr< MediaObjectImpl > parent, int garbagePeriod) :
  FilterImpl (parent, garbagePeriod)
{
  // TODO:
}

void
ChromaFilterImpl::setBackground (const std::string &uri)
{
  // TODO:
}

void
ChromaFilterImpl::unsetBackground ()
{
  // TODO:
}

std::shared_ptr<MediaObject>
ChromaFilter::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline,
  std::shared_ptr<WindowParam> window, const std::string &backgroundImage,
  int garbagePeriod)
{
  std::shared_ptr<MediaObject> object (new ChromaFilterImpl (
                                         window, backgroundImage,
                                         std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline), garbagePeriod) );

  return object;
}

ChromaFilterImpl::StaticConstructor ChromaFilterImpl::staticConstructor;

ChromaFilterImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
