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

#ifndef __JSONRPC_HANDLER_HPP__
#define __JSONRPC_HANDLER_HPP__

#include <string>
#include <memory>
#include <map>

#include <jsoncpp/json/json.h>
#include "JsonRpcException.hpp"

namespace kurento
{
namespace JsonRpc
{

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

#endif /* __JSONRPC_HANDLER_HPP__ */
