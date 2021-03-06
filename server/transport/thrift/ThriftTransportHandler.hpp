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

#ifndef __MEDIA_SERVER_TRANSPORT_HANDLER_HPP__
#define __MEDIA_SERVER_TRANSPORT_HANDLER_HPP__

#include "KmsMediaServerService.h"
#include <Processor.hpp>
#include <memory>

namespace kurento
{

class ThriftTransportHandler: public KmsMediaServerServiceIf
{
public:
  ThriftTransportHandler (const boost::property_tree::ptree &config,
                          std::shared_ptr<Processor> processor);
  ~ThriftTransportHandler ();

  /* JsonRPC */
  void invokeJsonRpc (std::string &_return, const std::string &request);

  virtual std::string processSubscription (std::shared_ptr<MediaObjectImpl> obj,
      const std::string &sessionId, const std::string &eventType,
      const Json::Value &params);

private:

  std::shared_ptr<Processor> processor;

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;
};

} // kurento

#endif /* __MEDIA_SERVER_TRANSPORT_HANDLER_HPP__ */
