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

#ifndef __MEDIA_SERVER_SERVICE_HANDLER_HPP__
#define __MEDIA_SERVER_SERVICE_HANDLER_HPP__

#include "KmsMediaServerService.h"
#include "types/MediaHandler.hpp"
#include "common/MediaSet.hpp"
#include "common/ConcurrentMap.hpp"
#include <module.hpp>

#include <jsoncpp/json/json.h>

namespace kurento
{

class MediaServerServiceHandler: public KmsMediaServerServiceIf
{
public:
  MediaServerServiceHandler (std::map <std::string, KurentoModule *> &modules);
  ~MediaServerServiceHandler ();

  /* JsonRPC */
  void invokeJsonRpc (std::string &_return, const std::string &request);

private:
  MediaSet mediaSet;
  std::map <std::string, KurentoModule *> &modules;

private:
  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;
};

} // kurento

#endif /* __MEDIA_SERVER_SERVICE_HANDLER_HPP__ */
