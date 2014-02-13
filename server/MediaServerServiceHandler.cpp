/*
 * (C) Copyright 2013 Kurento (http://kurento.org/)
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

#include "MediaServerServiceHandler.hpp"
#include "KmsMediaServer_constants.h"
#include <gst/gst.h>

#include "types/MediaPipeline.hpp"
#include "types/MediaElement.hpp"
#include "types/Mixer.hpp"

#include "utils/utils.hpp"

#define GST_CAT_DEFAULT kurento_media_server_service_handler
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoMediaServerServiceHandler"

namespace kurento
{

MediaServerServiceHandler::MediaServerServiceHandler (
  std::map <std::string, KurentoModule *> &modules) : modules (modules)
{
}

MediaServerServiceHandler::~MediaServerServiceHandler ()
{
}

void
MediaServerServiceHandler::invokeJsonRpc (std::string &_return,
    const std::string &request)
{
  // TODO:


//   Json::Reader reader;
//   Json::FastWriter writer;
//   Json::Value id;
//
//   try {
//     Json::Value jsonRequest;
//     Json::Value response;
//
//     reader.parse (request, jsonRequest);
//
//     GST_INFO ("JsonRpc request: %s", request.c_str() );
//
//     id = jsonRequest ["id"];
//     response["id"] = id;
//     response["error"];
//     response["result"];
//     response["jsonrpc"] = "2.0";
//
//     if (jsonRequest.empty() ) {
//       response ["error"] = "Error on request, empty or not parseable";
//     } else if (!jsonRequest["method"].isString() ) {
//       response ["error"] = "'method' is not a string";
//     } else if (jsonRequest["method"].asString() == "create") {
//       create (response, jsonRequest["params"]);
//     } else {
//       response ["error"] = "Method not supported";
//       response ["result"] = "Method not supported";
//     }
//
//     _return = writer.write (response);
//     GST_INFO ("JsonRpc return: %s", _return.c_str() );
//   } catch (...) {
//     Json::Value response;
//
//     response["result"] = "Unexpected exception";
//     response["error"] = "Unexpected exception";
//     response["jsonrpc"] = "2.0";
//     response["id"] = id;
//
//     GST_ERROR ("Unexpected exception");
//     _return = writer.write (response);
//   }
}


MediaServerServiceHandler::StaticConstructor
MediaServerServiceHandler::staticConstructor;

MediaServerServiceHandler::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} // kurento
