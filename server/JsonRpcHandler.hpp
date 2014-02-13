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

#ifndef JSONRPC_HANDLER_H
#define JSONRPC_HANDLER_H

#include <string>
#include <memory>
#include <map>

#include <jsoncpp/json/json.h>

namespace kurento
{
namespace JsonRpc
{

enum ErrorCode {
  PARSE_ERROR = -32700,
  INVALID_REQUEST = -32600,
  METHOD_NOT_FOUND = -32601,
  INVALID_PARAMS = -32602,
  INTERNAL_ERROR = -32603,
  SERVER_ERROR_INIT = -32000,
  SERVER_ERROR_END = -32099
};

class CallException
{

public:
  CallException (ErrorCode code, const std::string &message,
                 const Json::Value &data = Json::Value::null);

  ErrorCode getCode() {
    return code;
  }
  std::string getMessage() {
    return message;
  }
  Json::Value getData() {
    return data;
  }

private:
  ErrorCode code;
  std::string message;
  Json::Value data;
};

class Method
{
public:

  virtual ~Method() {};
  virtual void call (const Json::Value &msg,
                     Json::Value &response) throw (CallException) = 0;
  virtual std::string getName() const = 0;
};


class Handler
{
public:

  Handler() {};
  virtual ~Handler() {};

  void addMethod (std::shared_ptr<Method> method);
  bool process (const std::string &msg, std::string &_responseMsg);

private:

  Json::Reader reader;
  Json::FastWriter writer;

  std::map<std::string, std::shared_ptr<Method>> methods;
  bool process (const Json::Value &msg, Json::Value &_response);
  bool checkProtocol (const Json::Value &root, Json::Value &error);
};

} /* JsonRpc */
} /* kurento  */

#endif /* JSONRPC_HANDLER_H */

