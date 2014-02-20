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

#ifndef __HTTP_ENDPOINT_IMPL_HPP__
#define __HTTP_ENDPOINT_IMPL_HPP__

#include "SessionEndpointImpl.hpp"
#include <generated/HttpEndpoint.hpp>
#include "httpendpointserver.hpp"

namespace kurento
{

class HttpEndpointImpl: public virtual HttpEndpoint, public SessionEndpointImpl
{
public:
  HttpEndpointImpl (int disconnectionTimeout,
                    std::shared_ptr< MediaObjectImpl > parent,
                    int garbagePeriod);
  virtual ~HttpEndpointImpl() throw ();

  virtual std::string getUrl ();

protected:
  void register_end_point ();
  bool is_registered();

private:
  std::string url;
  bool urlSet = false;
  guint disconnectionTimeout;
  void setUrl (const std::string &);

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;

  gulong actionRequestedHandlerId;
  gulong urlRemovedHandlerId;
  gulong urlExpiredHandlerId;
  gint sessionStarted = 0;

  /* Functions that operate in main loop context */
  friend gboolean dispose_http_end_point (gpointer data);
  friend gboolean init_http_end_point (gpointer data);

  friend void http_end_point_raise_petition_event (HttpEndpointImpl *httpEp,
      KmsHttpEndPointAction action);
  friend void kurento_http_end_point_raise_session_terminated_event (
    HttpEndpointImpl *httpEp, const gchar *uri);
  friend void kurento_http_end_point_eos_detected_cb (GstElement *element,
      gpointer data);
};

} /* kurento */

#endif /* __HTTP_ENDPOINT_IMPL_HPP__ */
