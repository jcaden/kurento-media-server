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

#include <gst/gst.h>
#include <ServerMethods.hpp>
#include <types/MediaPipelineImpl.hpp>
#include <common/MediaSet.hpp>

#define GST_CAT_DEFAULT kurento_server_methods
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoServerMethods"

namespace kurento
{

void ServerMethods::CreateMethod::call (const Json::Value &msg,
                                        Json::Value &response)
throw (JsonRpc::CallException)
{
  methods->create (msg, response);
}

void ServerMethods::InvokeMethod::call (const Json::Value &msg,
                                        Json::Value &response)
throw (JsonRpc::CallException)
{
  methods->invoke (msg, response);
}


ServerMethods::ServerMethods() : methods (new Methods() )
{
  std::shared_ptr<JsonRpc::Method> createMethod (new CreateMethod (methods) );
  std::shared_ptr<JsonRpc::Method> invokeMethod (new InvokeMethod (methods) );

  handler.addMethod (createMethod);
  handler.addMethod (invokeMethod);
}


void
ServerMethods::process (const std::string &request, std::string &response)
{
  handler.process (request, response);
}

void
ServerMethods::Methods::invoke (const Json::Value &params,
                                Json::Value &response)
throw (JsonRpc::CallException)
{
  Json::FastWriter writer;

  GST_DEBUG ("invoke called: %s", writer.write (params).c_str() );
  // TODO: Invoke method
}

void
ServerMethods::Methods::create (const Json::Value &params,
                                Json::Value &response)
throw (JsonRpc::CallException)
{
  Json::FastWriter writer;
  std::string type;
  std::shared_ptr<Factory> factory;

  GST_DEBUG ("create called: %s", writer.write (params).c_str() );

  if (params == Json::Value::null) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'params' is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params.isMember ("type") ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'type' parameter is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params["type"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'type' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  type = params["type"].asString();

  if (!objectRegistrar) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "Class '" + type + "' does not exist");
    // TODO: Define error data and code
    throw e;
  }

  factory = objectRegistrar->getFactory (type);

  if (factory) {
    std::shared_ptr <MediaObjectImpl> object;

    object = std::dynamic_pointer_cast<MediaObjectImpl> (
               factory->createObject (params["constructorParams"]) );

    MediaSet::getMediaSet()->reg (object);
    response = object->getIdStr();
  } else {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "Class '" + type + "' does not exist");
    // TODO: Define error data and code
    throw e;
  }

}

ServerMethods::StaticConstructor ServerMethods::staticConstructor;

ServerMethods::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}



} /* kurento */
