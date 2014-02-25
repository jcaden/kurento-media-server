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
#include <string>
#include <EventHandler.hpp>

#define GST_CAT_DEFAULT kurento_server_methods
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoServerMethods"

namespace kurento
{

ServerMethods::ServerMethods()
{
  handler.addMethod ("create", std::bind (&ServerMethods::create, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2) );
  handler.addMethod ("invoke", std::bind (&ServerMethods::invoke, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2) );
  handler.addMethod ("subscribe", std::bind (&ServerMethods::subscribe, this,
                     std::placeholders::_1,
                     std::placeholders::_2) );
}

void
ServerMethods::process (const std::string &request, std::string &response)
{
  handler.process (request, response);
}

void
ServerMethods::subscribe (const Json::Value &params,
                          Json::Value &response) throw (JsonRpc::CallException)
{
  std::shared_ptr<MediaObject> obj;
  std::string eventType;
  std::string ip;
  int port;
  std::string sessionId;

  if (params == Json::Value::null) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'params' is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params["type"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'type' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  eventType = params["type"].asString();

  if (!params["ip"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'ip' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  ip = params["ip"].asString();

  if (!params["port"].isInt() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'port' parameter should be a int");
    // TODO: Define error data and code
    throw e;
  }

  port = params["port"].asInt();

  if (!params["object"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'object' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  try {
    std::shared_ptr<EventHandler> handler (new EventHandler (ip, port) );

    obj = MediaObject::Factory::getObject (params["object"].asString () );
    sessionId = obj->connect (eventType, handler);
    eventHandlers[sessionId] = handler;
  } catch (JsonRpc::CallException &ex) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "object not found: " + ex.getMessage() );
    throw e;
  }

  if (sessionId == "") {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "event not found");
    throw e;
  }

  response["sessionId"] = sessionId;
  response["object"] = params["object"];
}

void
ServerMethods::invoke (const Json::Value &params,
                       Json::Value &response)
throw (JsonRpc::CallException)
{
  std::shared_ptr<MediaObject> obj;

  if (params == Json::Value::null) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'params' is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params.isMember ("operation") ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'operation' parameter is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params["operation"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'operation' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  if (params.isMember ("operationParams") ) {
    if (!params["operationParams"].isObject() ) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'operation' parameter should be a string");
      // TODO: Define error data and code
      throw e;
    }
  }

  if (!params.isMember ("object") ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'object' parameter is requiered");
    // TODO: Define error data and code
    throw e;
  }

  if (!params["object"].isString() ) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'object' parameter should be a string");
    // TODO: Define error data and code
    throw e;
  }

  try {
    obj = MediaPipeline::Factory::getObject (params["object"].asString () );
  } catch (JsonRpc::CallException &ex) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "object not found: " + ex.getMessage() );
    throw e;
  }

  if (!obj) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "object not found");
    throw e;
  }

  try {
    obj->getInvoker().invoke (obj, params["operation"].asString(),
                              params["operationParams"], response);
  } catch (std::string &ex) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "Unexpected error: " + ex);
    throw e;
  } catch (std::exception &ex) {
    std::string message = "Unexpected exception: ";

    message.append (ex.what() );
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT, message);
    throw e;
  } catch (...) {
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "Unexpected exception");
    throw e;
  }
}

void
ServerMethods::create (const Json::Value &params,
                       Json::Value &response)
throw (JsonRpc::CallException)
{
  std::string type;
  std::shared_ptr<Factory> factory;

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
