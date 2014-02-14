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

#ifndef __SERVER_METHODS__
#define __SERVER_METHODS__

#include <JsonRpcHandler.hpp>

namespace kurento
{

class ServerMethods
{

public:
  ServerMethods();
  virtual ~ServerMethods() {};

  void process (const std::string &request, std::string &response);

private:

  class Methods
  {
  public:
    void create (const Json::Value &params,
                 Json::Value &response) throw (JsonRpc::CallException);
    void invoke (const Json::Value &params,
                 Json::Value &response) throw (JsonRpc::CallException);
  };

  class CreateMethod : public JsonRpc::Method
  {
  public:
    CreateMethod (std::shared_ptr<Methods> methods) : Method(),
      methods (methods) {} ;

    void call (const Json::Value &msg,
               Json::Value &response) throw (JsonRpc::CallException);
    std::string getName() const {
      return "create";
    }

  private:
    std::shared_ptr<Methods> methods;
  };

  class InvokeMethod : public JsonRpc::Method
  {
  public:
    InvokeMethod (std::shared_ptr<Methods> methods) : Method(),
      methods (methods) {};

    void call (const Json::Value &msg,
               Json::Value &response) throw (JsonRpc::CallException);
    std::string getName() const {
      return "invoke";

    }

  private:
    std::shared_ptr<Methods> methods;
  };

  JsonRpc::Handler handler;

  std::shared_ptr<Methods> methods;

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;
};

} /* kurento */

#endif /* __SERVER_METHODS__ */
