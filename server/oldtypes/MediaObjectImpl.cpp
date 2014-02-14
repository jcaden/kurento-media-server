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

#include "MediaObjectImpl.hpp"

#include "utils/utils.hpp"

#include <glibmm.h>

#define GST_CAT_DEFAULT kurento_media_object
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoMediaObject"

namespace kurento
{

// std::map<std::string, KmsMediaParam> MediaObjectImpl::emptyParams =
//   std::map<std::string, KmsMediaParam> ();
// KmsMediaEventData MediaObjectImpl::defaultKmsMediaEventData =
//   KmsMediaEventData ();

static uint64_t
createId()
{
  static Glib::Mutex mutex;
  static int seed_initiated = 0;
  uint64_t ret;

  mutex.lock();

  if (!seed_initiated) {
    srand48 (time (NULL) );
    seed_initiated = 1;
  }

  ret = lrand48();
  mutex.unlock();

  return ret;
}

// void
// MediaObjectImpl::init (const std::map<std::string, KmsMediaParam> &params,
//                        bool defaultCollectOnUnreferenced)
// {
//   const KmsMediaParam *p;
//   KmsMediaObjectConstructorParams mediaObjectParams;
//
//   p = getParam (params, g_KmsMediaObject_constants.CONSTRUCTOR_PARAMS_DATA_TYPE);
//
//   if (p != NULL) {
//     unmarshalKmsMediaObjectConstructorParams (mediaObjectParams, p->data);
//
//     if (mediaObjectParams.__isset.collectOnUnreferenced) {
//       this->collectOnUnreferenced = mediaObjectParams.collectOnUnreferenced;
//     } else {
//       this->collectOnUnreferenced = defaultCollectOnUnreferenced;
//     }
//
//     if (mediaObjectParams.__isset.garbageCollectorPeriod) {
//       this->garbageCollectorPeriod = mediaObjectParams.garbageCollectorPeriod;
//     }
//   }
//
//   this->excludeFromGC = (this->garbageCollectorPeriod <= 0);
//
//   GST_TRACE ("MediaObject %" G_GINT64_FORMAT
//              " excludeFromGC: %d, collectOnUnreferenced: %d", this->id,
//              this->excludeFromGC, this->collectOnUnreferenced);
// }

MediaObjectImpl::MediaObjectImpl (bool defaultCollectOnUnreferenced)
{
  id = createId();
//   generateUUID (token);
//   init (params, defaultCollectOnUnreferenced);
}

MediaObjectImpl::MediaObjectImpl (std::shared_ptr<MediaObjectImpl> parent,
                                  bool defaultCollectOnUnreferenced)
{
  id = createId();
//   this->token = parent->token;
  this->parent = parent;
//   init (params, defaultCollectOnUnreferenced);
}

MediaObjectImpl::~MediaObjectImpl() throw ()
{
}

std::shared_ptr<MediaObjectImpl>
MediaObjectImpl::getParent ()
{
  if (parent == NULL) {
    // TODO: Raise exception
//     KmsMediaServerException except;
//
//     createKmsMediaServerException (except,
//                                    g_KmsMediaErrorCodes_constants.MEDIA_OBJECT_HAS_NOT_PARENT, "No parent");
//     throw except;
  }

  return parent;
}

// void
// MediaObjectImpl::invoke (KmsMediaInvocationReturn &_return,
//                          const std::string &command,
//                          const std::map<std::string, KmsMediaParam> &params)
// throw (KmsMediaServerException)
// {
//   KmsMediaServerException except;
//
//   createKmsMediaServerException (except,
//                                  g_KmsMediaErrorCodes_constants.MEDIA_OBJECT_METHOD_NOT_FOUND,
//                                  "This media object has not any command named " + command);
//   throw except;
// }
//
// void
// MediaObjectImpl::subscribe (std::string &_return, const std::string &eventType,
//                             const std::string &handlerAddress,
//                             const int32_t handlerPort)
// throw (KmsMediaServerException)
// {
//   KmsMediaServerException except;
//
//   createKmsMediaServerException (except,
//                                  g_KmsMediaErrorCodes_constants.MEDIA_OBJECT_EVENT_NOT_SUPPORTED,
//                                  "This media object has not any event named " + eventType);
//   throw except;
// }
//
// void
// MediaObjectImpl::unsubscribe (const std::string &callbackToken) throw (
//   KmsMediaServerException)
// {
//   mediaHandlerManager.removeMediaHandler (callbackToken);
// }
//
// void
// MediaObjectImpl::sendEvent (const std::string &eventType,
//                             const KmsMediaEventData &eventData)
// {
//   std::shared_ptr<KmsMediaEvent> event (new KmsMediaEvent () );
//
//   event->__set_type (eventType);
//   event->__set_source (*this);
//
//   if (eventData != defaultKmsMediaEventData) {
//     event->__set_eventData (eventData);
//   }
//
//   mediaHandlerManager.sendEvent (event);
// }
//
// void
// MediaObjectImpl::subscribeError (std::string &_return,
//                                  const std::string &handlerAddress,
//                                  const int32_t handlerPort)
// throw (KmsMediaServerException)
// {
//   mediaHandlerManager.addMediaErrorHandler (_return, handlerAddress, handlerPort);
// }
//
// void
// MediaObjectImpl::unsubscribeError (const std::string &callbackToken)
// throw (KmsMediaServerException)
// {
//   mediaHandlerManager.removeMediaErrorHandler (callbackToken);
// }
//
// void
// MediaObjectImpl::sendError (const std::shared_ptr<KmsMediaError> &error)
// {
//   mediaHandlerManager.sendError (error);
// }
//
// void
// MediaObjectImpl::sendError (const std::string &errorType,
//                             const std::string &description, int32_t errorCode)
// {
//   std::shared_ptr<KmsMediaError> error (new KmsMediaError () );
//
//   error->__set_type (errorType);
//   error->__set_description (description);
//   error->__set_errorCode (errorCode);
//   error->__set_source (*this);
//
//   mediaHandlerManager.sendError (error);
//
//   try {
//     this->getParent()->sendError (error);
//   } catch (KmsMediaServerException e) {
//     GST_DEBUG ("No parent");
//   }
// }

std::string MediaObjectImpl::getIdStr ()
{
  std::string ret;
  gchar *id_str;

  id_str = g_strdup_printf ("%" G_GUINT64_FORMAT, id);
  ret = id_str;
  g_free (id_str);

  return ret;
}

uint64_t
MediaObjectImpl::getIdFromStr (std::string &strId) throw (std::invalid_argument)
{
  uint64_t id;
  int n;

  n = sscanf (strId.c_str(), "%" G_GUINT64_FORMAT, &id);

  if (n != 1) {
    throw std::invalid_argument ("strId is not a valid ID");
  }

  return id;
}


MediaObjectImpl::StaticConstructor MediaObjectImpl::staticConstructor;

MediaObjectImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} // kurento
