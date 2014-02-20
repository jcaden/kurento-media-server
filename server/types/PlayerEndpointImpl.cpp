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

#include "PlayerEndpointImpl.hpp"
#include <generated/MediaPipeline.hpp>

#define GST_CAT_DEFAULT kurento_player_endpoint_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoPlayerEndpointImpl"

#define FACTORY_NAME "playerendpoint"

namespace kurento
{

void
player_eos (GstElement *player, gpointer data)
{
  // TODO Send event
}

void
player_invalid_uri (GstElement *player, gpointer data)
{
  // TODO Send error
}

void
player_invalid_media (GstElement *player, gpointer data)
{
  // TODO Send error
}

PlayerEndpointImpl::PlayerEndpointImpl (bool useEncodedMedia,
                                        const std::string &uri,
                                        std::shared_ptr< MediaObjectImpl > parent,
                                        int garbagePeriod) :
  UriEndpointImpl (uri, FACTORY_NAME,
                   parent, garbagePeriod)
{
  GstElement *element = getGstreamerElement();

  g_object_set (G_OBJECT (element), "use-encoded-media", useEncodedMedia, NULL);

  g_signal_connect (element, "eos", G_CALLBACK (player_eos), this);
  g_signal_connect (element, "invalid-uri", G_CALLBACK (player_invalid_uri),
                    NULL);
  g_signal_connect (element, "invalid-media", G_CALLBACK (player_invalid_media),
                    NULL);
}

PlayerEndpointImpl::~PlayerEndpointImpl()
{
}


void
PlayerEndpointImpl::play ()
{
  start();
}

std::shared_ptr<MediaObject>
PlayerEndpoint::Factory::createObject (
  std::shared_ptr<MediaPipeline> mediaPipeline, const std::string &uri,
  bool useEncodedMedia, int garbagePeriod)
{
  std::shared_ptr<MediaObject> obj (
    new PlayerEndpointImpl (useEncodedMedia, uri,
                            std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline),
                            garbagePeriod) );

  return obj;
}


PlayerEndpointImpl::StaticConstructor PlayerEndpointImpl::staticConstructor;

PlayerEndpointImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
