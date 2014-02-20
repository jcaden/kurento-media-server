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

#include "HttpEndpointImpl.hpp"

#define GST_CAT_DEFAULT kurento_http_endpoint_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoHttpEndpointImpl"

#define FACTORY_NAME "httpendpoint"

namespace kurento
{

void
http_end_point_raise_petition_event (HttpEndpointImpl *httpEp,
                                     KmsHttpEndPointAction action)
{
  if (!g_atomic_int_compare_and_exchange (& (httpEp->sessionStarted), 0, 1) ) {
    return;
  }

  if (action == KMS_HTTP_END_POINT_ACTION_UNDEFINED) {
    GST_ERROR ("Invalid or unexpected request received");
    // TODO: Raise error to remote client
    return;
  }

  // TODO: Sent session start event
}

static void
action_requested_cb (KmsHttpEPServer *server, const gchar *uri,
                     KmsHttpEndPointAction action, gpointer data)
{
  HttpEndpointImpl *httpEp = (HttpEndpointImpl *) data;
  std::string uriStr = uri;
  std::string url = httpEp->getUrl();

  GST_DEBUG ("Action requested URI %s", uriStr.c_str() );

  if (url.size() <= uriStr.size() ) {
    return;
  }

  /* Remove the initial "http://host:port" to compare the uri */
  std::string substr = url.substr (url.size() - uriStr.size(),
                                   std::string::npos);

  if (substr.compare (uriStr) != 0) {
    return;
  }

  http_end_point_raise_petition_event (httpEp, action);
}

static std::string
getUriFromUrl (std::string url)
{
  std::string uri;
  gboolean host_read = FALSE;

  /* skip first 7 characters in the url regarding the protocol "http://" */
  if (url.size() < 7) {
    GST_ERROR ("Invalid URL %s", url.c_str() );
    return NULL;
  }

  for ( guint i = 7; i < url.size(); i++) {
    gchar c = url.at (i);

    if (!host_read) {
      if (c == '/') {
        /* URL has no port */
        uri = url.substr (i, std::string::npos);
        break;
      } else if (c == ':') {
        /* skip port number */
        host_read = TRUE;
        continue;
      } else {
        continue;
      }
    }

    if (c != '/') {
      continue;
    }

    uri = url.substr (i, std::string::npos);
    break;
  }

  return uri;
}

void
kurento_http_end_point_raise_session_terminated_event (HttpEndpointImpl *httpEp,
    const gchar *uri)
{
  std::string uriStr = uri;

  if (httpEp->url.size() <= uriStr.size() ) {
    return;
  }

  /* Remove the initial "http://host:port" to compare the uri */
  std::string substr = httpEp->url.substr (httpEp->url.size() - uriStr.size(),
                       std::string::npos);

  if (substr.compare (uriStr) != 0) {
    return;
  }

  GST_DEBUG ("Session terminated URI %s", uriStr.c_str() );

  if (!g_atomic_int_compare_and_exchange (& (httpEp->sessionStarted), 1, 0) ) {
    return;
  }

  // TODO: Send event media session complete
}

static void
url_removed_cb (KmsHttpEPServer *server, const gchar *uri, gpointer data)
{
  kurento_http_end_point_raise_session_terminated_event ( (
        HttpEndpointImpl *) data,
      uri);
}

static void
url_expired_cb (KmsHttpEPServer *server, const gchar *uri, gpointer data)
{
  kurento_http_end_point_raise_session_terminated_event ( (
        HttpEndpointImpl *) data,
      uri);
}

struct MainLoopData {
  gpointer data;
  GSourceFunc func;
  GDestroyNotify destroy;
  Glib::Mutex *mutex;
};

static gboolean
main_loop_wrapper_func (gpointer data)
{
  struct MainLoopData *mdata = (struct MainLoopData *) data;

  return mdata->func (mdata->data);
}

static void
main_loop_data_destroy (gpointer data)
{
  struct MainLoopData *mdata = (struct MainLoopData *) data;

  if (mdata->destroy != NULL) {
    mdata->destroy (mdata->data);
  }

  mdata->mutex->unlock();
  g_slice_free (struct MainLoopData, mdata);
}

static void
operate_in_main_loop_context (GSourceFunc func, gpointer data,
                              GDestroyNotify destroy)
{
  struct MainLoopData *mdata;
  Glib::Mutex mutex;

  mdata = g_slice_new (struct MainLoopData);
  mdata->data = data;
  mdata->func = func;
  mdata->destroy = destroy;
  mdata->mutex = &mutex;

  mutex.lock ();
  g_idle_add_full (G_PRIORITY_HIGH_IDLE, main_loop_wrapper_func, mdata,
                   main_loop_data_destroy);
  mutex.lock ();
}

gboolean
init_http_end_point (gpointer data)
{
  HttpEndpointImpl *httpEp = (HttpEndpointImpl *) data;
  std::string uri;
  const gchar *url;
  gchar *c_uri;
  gchar *addr;
  guint port;

  httpEp->actionRequestedHandlerId = g_signal_connect (httpepserver,
                                     "action-requested",
                                     G_CALLBACK (action_requested_cb), httpEp);
  httpEp->urlRemovedHandlerId = g_signal_connect (httpepserver, "url-removed",
                                G_CALLBACK (url_removed_cb), httpEp);
  httpEp->urlExpiredHandlerId = g_signal_connect (httpepserver, "url-expired",
                                G_CALLBACK (url_expired_cb), httpEp);

  url = kms_http_ep_server_register_end_point (httpepserver, httpEp->element,
        httpEp->disconnectionTimeout);

  if (url == NULL) {
    return G_SOURCE_REMOVE;
  }

  g_object_get (G_OBJECT (httpepserver), "announced-address", &addr, "port",
                &port,
                NULL);
  c_uri = g_strdup_printf ("http://%s:%d%s", addr, port, url);
  uri = c_uri;

  g_free (addr);
  g_free (c_uri);

  httpEp->setUrl (uri);
  httpEp->urlSet = true;

  return G_SOURCE_REMOVE;
}

gboolean
dispose_http_end_point (gpointer data)
{
  HttpEndpointImpl *httpEp = (HttpEndpointImpl *) data;

  g_signal_handler_disconnect (httpepserver, httpEp->actionRequestedHandlerId);
  g_signal_handler_disconnect (httpepserver, httpEp->urlExpiredHandlerId);
  g_signal_handler_disconnect (httpepserver, httpEp->urlRemovedHandlerId);

  std::string uri = getUriFromUrl (httpEp->url);

  if (!uri.empty() ) {
    kms_http_ep_server_unregister_end_point (httpepserver, uri.c_str() );
  }

  return FALSE;
}

void
HttpEndpointImpl::register_end_point ()
{
  operate_in_main_loop_context (init_http_end_point, this, NULL);
}

bool
HttpEndpointImpl::is_registered()
{
  return urlSet;
}


HttpEndpointImpl::HttpEndpointImpl (int disconnectionTimeout,
                                    std::shared_ptr< MediaObjectImpl > parent,
                                    int garbagePeriod) :
  SessionEndpointImpl (FACTORY_NAME, parent, garbagePeriod)
{
  this->disconnectionTimeout = disconnectionTimeout;
}

HttpEndpointImpl::~HttpEndpointImpl()
{
  operate_in_main_loop_context (dispose_http_end_point, this, NULL);
}

std::string
HttpEndpointImpl::getUrl ()
{
  return url;
}

void
HttpEndpointImpl::setUrl (const std::string &newUrl)
{
  url = newUrl;
}

HttpEndpointImpl::StaticConstructor HttpEndpointImpl::staticConstructor;

HttpEndpointImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
