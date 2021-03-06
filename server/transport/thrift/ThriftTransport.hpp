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

#ifndef __THRIFT_TRANSPORT_HPP__
#define __THRIFT_TRANSPORT_HPP__

#include <Transport.hpp>
#include <server/TNonblockingServer.h>
#include <glibmm/thread.h>
#include <Processor.hpp>

namespace kurento
{

class ThriftTransport: public Transport
{
public:
  ThriftTransport (const boost::property_tree::ptree &config,
                   std::shared_ptr<Processor> processor);
  virtual ~ThriftTransport() throw ();
  virtual void start ();
  virtual void stop ();

private:
  int port;
  std::shared_ptr<apache::thrift::server::TNonblockingServer> server;
  Glib::Thread *thread;

  const boost::property_tree::ptree &config;
  std::shared_ptr<Processor> processor;

  void serve ();

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;
};

} /* kurento */

#endif /* __THRIFT_TRANSPORT_HPP__ */
