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

#include "MediaServerServiceHandler.hpp"
#include "KmsMediaServer_constants.h"
#include <gst/gst.h>

#include "types/MediaPipeline.hpp"
#include "types/MediaElement.hpp"
#include "types/Mixer.hpp"

#include "KmsMediaErrorCodes_constants.h"
#include "utils/utils.hpp"

#define GST_CAT_DEFAULT kurento_media_server_service_handler
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoMediaServerServiceHandler"

namespace kurento
{

MediaServerServiceHandler::MediaServerServiceHandler (
  std::map <std::string, KurentoModule *> &modules) : modules (modules)
{
}

MediaServerServiceHandler::~MediaServerServiceHandler ()
{
}

int32_t
MediaServerServiceHandler::getVersion ()
{
  return g_KmsMediaServer_constants.VERSION;
}

/* MediaObject */

void
MediaServerServiceHandler::keepAlive (const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  GST_TRACE ("keepAlive %" G_GINT64_FORMAT, mediaObjectRef.id);

  try {
    mediaSet.keepAlive (mediaObjectRef);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("keepAlive %" G_GINT64_FORMAT " throws KmsMediaServerException (%s)",
               mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("keepAlive %" G_GINT64_FORMAT " throws KmsMediaServerException",
               mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in keepAlive");
    throw except;
  }

  GST_TRACE ("keepAlive %" G_GINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::release (const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  GST_TRACE ("release %" G_GINT64_FORMAT, mediaObjectRef.id);

  try {
    mediaSet.unreg (mediaObjectRef);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("release %" G_GINT64_FORMAT " throws KmsMediaServerException (%s)",
               mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("release %" G_GINT64_FORMAT " throws KmsMediaServerException",
               mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in release");
    throw except;
  }

  GST_TRACE ("release %" G_GINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::subscribeEvent (std::string &_return,
    const KmsMediaObjectRef &mediaObjectRef,
    const std::string &eventType, const std::string &handlerAddress,
    const int32_t handlerPort) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GINT64_FORMAT,
             eventType.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->subscribe (_return, eventType, handlerAddress, handlerPort);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", eventType.c_str (), mediaObjectRef.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", eventType.c_str (), mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in subscribe");
    throw except;
  }

  GST_TRACE ("subscribe for '%s' event type in mediaObjectRef: %" G_GINT64_FORMAT
             " done", eventType.c_str (), mediaObjectRef.id);
}

void
MediaServerServiceHandler::unsubscribeEvent (const KmsMediaObjectRef
    &mediaObjectRef, const std::string &callbackToken)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %"
             G_GINT64_FORMAT, callbackToken.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->unsubscribe (callbackToken);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %"
               G_GINT64_FORMAT " throws KmsMediaServerException (%s)", callbackToken.c_str (),
               mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %"
               G_GINT64_FORMAT " throws KmsMediaServerException", callbackToken.c_str (),
               mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in subscribe");
    throw except;
  }

  GST_TRACE ("unsubscribe for '%s' callbackToken in mediaObjectRef: %"
             G_GINT64_FORMAT " done", callbackToken.c_str (), mediaObjectRef.id);
}

void
MediaServerServiceHandler::subscribeError (std::string &_return,
    const KmsMediaObjectRef &mediaObjectRef,
    const std::string &handlerAddress, const int32_t handlerPort)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("subscribe for errors in mediaObjectRef: %" G_GINT64_FORMAT,
             mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->subscribeError (_return, handlerAddress, handlerPort);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("subscribe for errors in mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)",
               mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("subscribe for errors in mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in subscribe");
    throw except;
  }

  GST_TRACE ("subscribe for errors in mediaObjectRef: %" G_GINT64_FORMAT
             " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::unsubscribeError (const KmsMediaObjectRef
    &mediaObjectRef, const std::string &callbackToken)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("unsubscribeError for '%s' callbackToken in mediaObjectRef: %"
             G_GINT64_FORMAT, callbackToken.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->unsubscribeError (callbackToken);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("unsubscribeError for '%s' callbackToken in mediaObjectRef: %"
               G_GINT64_FORMAT " throws KmsMediaServerException (%s)",
               callbackToken.c_str (), mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("unsubscribeError for '%s' callbackToken in mediaObjectRef: %"
               G_GINT64_FORMAT " throws KmsMediaServerException",
               callbackToken.c_str (), mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in subscribe");
    throw except;
  }

  GST_TRACE ("unsubscribeError for '%s' callbackToken in mediaObjectRef: %"
             G_GINT64_FORMAT " done", callbackToken.c_str (),
             mediaObjectRef.id);
}

void
MediaServerServiceHandler::invoke (KmsMediaInvocationReturn &_return,
                                   const KmsMediaObjectRef &mediaObjectRef, const std::string &command,
                                   const std::map<std::string, KmsMediaParam> &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;

  GST_TRACE ("invoke '%s' for mediaObjectRef: %" G_GINT64_FORMAT,
             command.c_str (), mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    mo->invoke (_return, command, params);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("invoke '%s' for mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", command.c_str (), mediaObjectRef.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("invoke '%s' for mediaObjectRef: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", command.c_str (), mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in invoke");
    throw except;
  }

  GST_TRACE ("invoke '%s' for mediaObjectRef: %" G_GINT64_FORMAT " done",
             command.c_str (), mediaObjectRef.id);
}

void
MediaServerServiceHandler::getParent (KmsMediaObjectRef &_return,
                                      const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo;
  std::shared_ptr<MediaObjectImpl> parent;

  GST_TRACE ("getParent %" G_GINT64_FORMAT, mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);
    parent = mo->getParent ();
    mediaSet.reg (parent);
    _return = *parent;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getParent %" G_GINT64_FORMAT " throws KmsMediaServerException (%s)",
               mediaObjectRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("release %" G_GINT64_FORMAT " throws KmsMediaServerException",
               mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getParent");
    throw except;
  }

  GST_TRACE ("getParent %" G_GINT64_FORMAT " done", mediaObjectRef.id);
}

void
MediaServerServiceHandler::getMediaPipeline (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mediaObjectRef)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaObjectImpl> mo, mediaPipeline;

  GST_TRACE ("getMediaPipeline %" G_GINT64_FORMAT, mediaObjectRef.id);

  try {
    mo = mediaSet.getMediaObject<MediaObjectImpl> (mediaObjectRef);

    mediaPipeline = mo->getPipeline ();

    mediaSet.reg (mediaPipeline);
    _return = *mediaPipeline;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaPipeline %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", mediaObjectRef.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("release %" G_GINT64_FORMAT " throws KmsMediaServerException",
               mediaObjectRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaPipeline");
    throw except;
  }

  GST_TRACE ("getMediaPipeline %" G_GINT64_FORMAT " done", mediaObjectRef.id);
}

/* MediaPipeline */

void
MediaServerServiceHandler::createMediaPipeline (KmsMediaObjectRef &_return)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mediaPipeline;

  GST_TRACE ("createMediaPipeline");

  try {
    mediaPipeline = std::shared_ptr<MediaPipeline> (new MediaPipeline (modules,
                    mediaSet) );
    GST_DEBUG ("createMediaPipeline id: %" G_GINT64_FORMAT ", token: %s",
               mediaPipeline->id, mediaPipeline->token.c_str() );
    mediaSet.reg (mediaPipeline);

    _return = *mediaPipeline;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaPipeline throws KmsMediaServerException");
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaPipeline");
    throw except;
  }

  GST_TRACE ("createMediaPipeline done");
}

void
MediaServerServiceHandler::createMediaPipelineWithParams (
  KmsMediaObjectRef &_return,
  const std::map<std::string, KmsMediaParam> &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaHandler> mh;
  std::shared_ptr<MediaPipeline> mediaPipeline;

  GST_TRACE ("createMediaPipelineWithParams");

  try {
    mediaPipeline = std::shared_ptr<MediaPipeline> (new MediaPipeline (modules,
                    mediaSet, params) );
    GST_DEBUG ("createMediaPipelineWithParams id: %" G_GINT64_FORMAT ", token: %s",
               mediaPipeline->id, mediaPipeline->token.c_str() );
    mediaSet.reg (mediaPipeline);

    _return = *mediaPipeline;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaPipelineWithParams throws KmsMediaServerException");
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaPipelineWithParams");
    throw except;
  }

  GST_TRACE ("createMediaPipelineWithParams done");
}

void
MediaServerServiceHandler::createMediaElement (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mediaPipeline,
    const std::string &elementType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<MediaElement> me;

  GST_TRACE ("createMediaElement pipeline: %" G_GINT64_FORMAT ", type: %s",
             mediaPipeline.id, elementType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    me = mp->createMediaElement (elementType);

    _return = *me;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaElement pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id,
               elementType.c_str (), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaElement pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException", mediaPipeline.id,
               elementType.c_str () );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaElement");
    throw except;
  }

  GST_TRACE ("createMediaElement pipeline: %" G_GINT64_FORMAT ", type: %s done",
             mediaPipeline.id, elementType.c_str () );
}

void
MediaServerServiceHandler::createMediaElementWithParams (
  KmsMediaObjectRef &_return, const KmsMediaObjectRef &mediaPipeline,
  const std::string &elementType,
  const std::map<std::string, KmsMediaParam> &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<MediaElement> me;

  GST_TRACE ("createMediaElementWithParams pipeline: %" G_GINT64_FORMAT
             ", type: %s", mediaPipeline.id, elementType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    me = mp->createMediaElement (elementType, params);

    _return = *me;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaElementWithParams pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id,
               elementType.c_str (), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaElementWithParams pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException", mediaPipeline.id,
               elementType.c_str () );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaElementWithParams");
    throw except;
  }

  GST_TRACE ("createMediaElementWithParams pipeline: %" G_GINT64_FORMAT
             ", type: %s done", mediaPipeline.id, elementType.c_str () );
}

void
MediaServerServiceHandler::createMediaMixer (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mediaPipeline,
    const std::string &mixerType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<Mixer> mixer;

  GST_TRACE ("createMediaMixer pipeline: %" G_GINT64_FORMAT ", type: %s",
             mediaPipeline.id, mixerType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    mixer = mp->createMediaMixer (mixerType);

    _return = *mixer;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaMixer pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id,
               mixerType.c_str (), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaMixer pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException", mediaPipeline.id,
               mixerType.c_str () );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaMixer");
    throw except;
  }

  GST_TRACE ("createMediaMixer pipeline: %" G_GINT64_FORMAT ", type: %s done",
             mediaPipeline.id, mixerType.c_str () );
}

void
MediaServerServiceHandler::createMediaMixerWithParams (KmsMediaObjectRef
    &_return, const KmsMediaObjectRef &mediaPipeline,
    const std::string &mixerType,
    const std::map<std::string, KmsMediaParam> &params)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPipeline> mp;
  std::shared_ptr<Mixer> mixer;

  GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GINT64_FORMAT
             ", type: %s", mediaPipeline.id, mixerType.c_str () );

  try {
    mp = mediaSet.getMediaObject<MediaPipeline> (mediaPipeline);
    mixer = mp->createMediaMixer (mixerType, params);

    _return = *mixer;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException (%s)", mediaPipeline.id,
               mixerType.c_str (), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GINT64_FORMAT
               ", type: %s throws KmsMediaServerException", mediaPipeline.id,
               mixerType.c_str () );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMediaMixerWithParams");
    throw except;
  }

  GST_TRACE ("createMediaMixerWithParams pipeline: %" G_GINT64_FORMAT
             ", type: %s done", mediaPipeline.id, mixerType.c_str () );
}

/* MediaElement */

void
MediaServerServiceHandler::getMediaSrcs (std::vector<KmsMediaObjectRef>
    &_return, const KmsMediaObjectRef &mediaElement)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSrc> > mediaSrcs;
  std::vector< std::shared_ptr<MediaSrc> >::iterator it;

  try {
    GST_TRACE ("getMediaSrcs element: %" G_GINT64_FORMAT, mediaElement.id);
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    me->getMediaSrcs (mediaSrcs);

    for ( it = mediaSrcs.begin() ; it != mediaSrcs.end(); ++it) {
      _return.push_back (**it);
    }

    GST_TRACE ("getMediaSrcs element: %" G_GINT64_FORMAT " done", mediaElement.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaSrcs element: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", mediaElement.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getMediaSrcs element: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaElement.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaSrcs");
    throw except;
  }
}

void
MediaServerServiceHandler::getMediaSinks (std::vector<KmsMediaObjectRef>
    &_return, const KmsMediaObjectRef &mediaElement)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSink> > mediaSinks;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getMediaSinks element: %" G_GINT64_FORMAT, mediaElement.id);
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    me->getMediaSinks (mediaSinks);

    for ( it = mediaSinks.begin() ; it != mediaSinks.end(); ++it) {
      _return.push_back (**it);
    }

    GST_TRACE ("getMediaSinks element: %" G_GINT64_FORMAT " done", mediaElement.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaSinks element: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", mediaElement.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getMediaSinks element: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaElement.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaSinks");
    throw except;
  }
}

void
MediaServerServiceHandler::getMediaSrcsByMediaType (
  std::vector<KmsMediaObjectRef> &_return, const KmsMediaObjectRef &mediaElement,
  const KmsMediaType::type mediaType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSrc> > mediaSrcs;
  std::vector< std::shared_ptr<MediaSrc> >::iterator it;

  try {
    GST_TRACE ("getMediaSrcByType element: %" G_GINT64_FORMAT " mediaType: %s",
               mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    me->getMediaSrcsByMediaType (mediaSrcs, mediaType);

    for ( it = mediaSrcs.begin() ; it != mediaSrcs.end(); ++it) {
      _return.push_back (**it);
    }

    GST_TRACE ("getMediaSrcsByType element: %" G_GINT64_FORMAT
               " mediaType: %s done", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaSrcsByType element: %" G_GINT64_FORMAT
               " mediaType: %s throws KmsMediaServerException (%s)", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getMediaSrcsByType element: %" G_GINT64_FORMAT
               " mediaType: %s throws KmsMediaServerException", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaSrcsByType");
    throw except;
  }
}

void
MediaServerServiceHandler::getMediaSinksByMediaType (
  std::vector<KmsMediaObjectRef> &_return, const KmsMediaObjectRef &mediaElement,
  const KmsMediaType::type mediaType) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> me;
  std::vector < std::shared_ptr<MediaSink> > mediaSinks;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getMediaSinksByType element: %" G_GINT64_FORMAT " mediaType: %s",
               mediaElement.id, _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    me = mediaSet.getMediaObject<MediaElement> (mediaElement);
    me->getMediaSinksByMediaType (mediaSinks, mediaType);

    for ( it = mediaSinks.begin() ; it != mediaSinks.end(); ++it) {
      _return.push_back (**it);
    }

    GST_TRACE ("getMediaSinksByType element: %" G_GINT64_FORMAT
               " mediaType: %s done", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaSinksByType element: %" G_GINT64_FORMAT
               " mediaType: %s throws KmsMediaServerException (%s)", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getMediaSinksByType element: %" G_GINT64_FORMAT
               " mediaType: %s throws KmsMediaServerException", mediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaSinksByType");
    throw except;
  }
}

void
MediaServerServiceHandler::getMediaSrcsByFullDescription (
  std::vector<KmsMediaObjectRef> &_return, const KmsMediaObjectRef &mediaElement,
  const KmsMediaType::type mediaType, const std::string &description)
throw (KmsMediaServerException)
{
  KmsMediaServerException except;

  GST_WARNING ("TODO: implement");
  createKmsMediaServerException (except,
                                 g_KmsMediaErrorCodes_constants.NOT_IMPLEMENTED,
                                 "Not implemented");
  throw except;
}

void
MediaServerServiceHandler::getMediaSinksByFullDescription (
  std::vector<KmsMediaObjectRef> &_return, const KmsMediaObjectRef &mediaElement,
  const KmsMediaType::type mediaType, const std::string &description)
throw (KmsMediaServerException)
{
  KmsMediaServerException except;

  GST_WARNING ("TODO: implement");
  createKmsMediaServerException (except,
                                 g_KmsMediaErrorCodes_constants.NOT_IMPLEMENTED,
                                 "Not implemented");
  throw except;
}

void
MediaServerServiceHandler::connectElements (const KmsMediaObjectRef
    &srcMediaElement, const KmsMediaObjectRef &sinkMediaElement)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> sink;
  std::shared_ptr<MediaElement> src;

  try {
    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT, srcMediaElement.id,
               sinkMediaElement.id);
    sink = mediaSet.getMediaObject<MediaElement> (sinkMediaElement);
    src = mediaSet.getMediaObject<MediaElement> (srcMediaElement);

    src->connect (sink);

    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT " done",
               srcMediaElement.id, sinkMediaElement.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT
               " throws KmsMediaServerException (%s)", srcMediaElement.id,
               sinkMediaElement.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", srcMediaElement.id,
               sinkMediaElement.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in connectElements");
    throw except;
  }
}

void
MediaServerServiceHandler::connectElementsByMediaType (const KmsMediaObjectRef
    &srcMediaElement,
    const KmsMediaObjectRef &sinkMediaElement,
    const KmsMediaType::type mediaType)
throw (KmsMediaServerException)
{
  std::shared_ptr<MediaElement> sink;
  std::shared_ptr<MediaElement> src;

  try {
    GST_TRACE ("connectElementsByMediaType srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT " mediaType: %s",
               srcMediaElement.id, sinkMediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    sink = mediaSet.getMediaObject<MediaElement> (sinkMediaElement);
    src = mediaSet.getMediaObject<MediaElement> (srcMediaElement);

    src->connect (sink, mediaType);

    GST_TRACE ("connectElementsByMediaType srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT " mediaType: %s done",
               srcMediaElement.id, sinkMediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT " mediaType: %s"
               " throws KmsMediaServerException (%s)", srcMediaElement.id,
               sinkMediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType), e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("connectElements srcMediaElement: %" G_GINT64_FORMAT
               " sinkMediaElement: %" G_GINT64_FORMAT " mediaType: %s"
               " throws KmsMediaServerException", srcMediaElement.id,
               sinkMediaElement.id,
               _KmsMediaType_VALUES_TO_NAMES.at (mediaType) );
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in connectElements");
    throw except;
  }
}

void
MediaServerServiceHandler::connectElementsByFullDescription (
  const KmsMediaObjectRef &srcMediaElement,
  const KmsMediaObjectRef &sinkMediaElement,
  const KmsMediaType::type mediaType,
  const std::string &mediaDescription)
throw (KmsMediaServerException)
{
  KmsMediaServerException except;

  GST_WARNING ("TODO: implement");
  createKmsMediaServerException (except,
                                 g_KmsMediaErrorCodes_constants.NOT_IMPLEMENTED,
                                 "Not implemented");
  throw except;
}

/* MediaPad */

void
MediaServerServiceHandler::getMediaElement (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mediaPadRef) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaPad> pad;

  try {
    GST_TRACE ("getMediaElement of pad: %" G_GINT64_FORMAT, mediaPadRef.id);
    pad = mediaSet.getMediaObject<MediaPad> (mediaPadRef);
    _return = * (pad->getMediaElement () );
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getMediaElement of pad: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mediaPadRef.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;
    GST_TRACE ("getMediaElement of pad: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaPadRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getMediaElement");
    throw except;
  }

  GST_TRACE ("getMediaElement of pad: %" G_GINT64_FORMAT " done", mediaPadRef.id);
}

/* MediaSrc */

void
MediaServerServiceHandler::connect (const KmsMediaObjectRef &mediaSrc,
                                    const KmsMediaObjectRef &mediaSink) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("connect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT,
               mediaSrc.id, mediaSink.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSink);
    src->connect (sink);
    GST_TRACE ("connect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT " done",
               mediaSrc.id, mediaSink.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("connect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mediaSrc.id, mediaSink.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("connect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaSrc.id, mediaSink.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR, "Unexpected error in connect");
    throw except;
  }
}

void
MediaServerServiceHandler::disconnect (const KmsMediaObjectRef &mediaSrc,
                                       const KmsMediaObjectRef &mediaSink) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("disconnect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT,
               mediaSrc.id, mediaSink.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSink);
    src->disconnect (sink);
    GST_TRACE ("disconnect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT
               " done", mediaSrc.id, mediaSink.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("disconnect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mediaSrc.id, mediaSink.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;
    GST_TRACE ("disconnect src: %" G_GINT64_FORMAT " sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaSrc.id, mediaSink.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in disconnect");
    throw except;
  }
}

void
MediaServerServiceHandler::getConnectedSinks (std::vector<KmsMediaObjectRef>
    &_return, const KmsMediaObjectRef &mediaSrc) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSrc> src;
  std::vector < std::shared_ptr<MediaSink> > mediaSinks;
  std::vector< std::shared_ptr<MediaSink> >::iterator it;

  try {
    GST_TRACE ("getConnectedSinks src: %" G_GINT64_FORMAT, mediaSrc.id);
    src = mediaSet.getMediaObject<MediaSrc> (mediaSrc);
    src->getConnectedSinks (mediaSinks);

    for ( it = mediaSinks.begin() ; it != mediaSinks.end(); ++it) {
      _return.push_back (**it);
    }

    GST_TRACE ("getConnectedSinks src: %" G_GINT64_FORMAT " done", mediaSrc.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getConnectedSinks src: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mediaSrc.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getConnectedSinks src: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaSrc.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getConnectedSinks");
    throw except;
  }
}

/* MediaSink */

void
MediaServerServiceHandler::getConnectedSrc (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mediaSinkRef) throw (KmsMediaServerException)
{
  std::shared_ptr<MediaSink> sink;

  try {
    GST_TRACE ("getConnectedSrc sink: %" G_GINT64_FORMAT, mediaSinkRef.id);
    sink = mediaSet.getMediaObject<MediaSink> (mediaSinkRef);
    _return = * (sink->getConnectedSrc() );
    GST_TRACE ("getConnectedSrc sink: %" G_GINT64_FORMAT " done", mediaSinkRef.id);
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("getConnectedSrc sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mediaSinkRef.id,
               e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("getConnectedSrc sink: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mediaSinkRef.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in getConnectedSrc");
    throw except;
  }
}

/* Mixer */

void
MediaServerServiceHandler::createMixerPort (KmsMediaObjectRef &_return,
    const KmsMediaObjectRef &mixer) throw (KmsMediaServerException)
{
  std::shared_ptr<Mixer> m;
  std::shared_ptr<MixerPort> mixerPort;

  GST_TRACE ("createMixerPort mixer: %" G_GINT64_FORMAT, mixer.id);

  try {
    m = mediaSet.getMediaObject<Mixer> (mixer);
    mixerPort = m->createMixerPort ();
    mediaSet.reg (mixerPort);

    _return = *mixerPort;
  } catch (const KmsMediaServerException &e) {
    GST_TRACE ("createMixerPort mixer: %" G_GINT64_FORMAT
               " throws KmsMediaServerException(%s)", mixer.id, e.description.c_str () );
    throw e;
  } catch (...) {
    KmsMediaServerException except;

    GST_TRACE ("createMixerPort mixer: %" G_GINT64_FORMAT
               " throws KmsMediaServerException", mixer.id);
    createKmsMediaServerException (except,
                                   g_KmsMediaErrorCodes_constants.UNEXPECTED_ERROR,
                                   "Unexpected error in createMixerPort");
    throw except;
  }

  GST_TRACE ("createMixerPort mixer: %" G_GINT64_FORMAT " done", mixer.id);
}

void
MediaServerServiceHandler::createMixerPortWithParams (
  KmsMediaObjectRef &_return, const KmsMediaObjectRef &mixer,
  const std::map<std::string, KmsMediaParam> &params)
throw (KmsMediaServerException)
{
  KmsMediaServerException except;

  GST_WARNING ("TODO: implement");
  createKmsMediaServerException (except,
                                 g_KmsMediaErrorCodes_constants.NOT_IMPLEMENTED,
                                 "Not implemented");
  throw except;
}

void
MediaServerServiceHandler::create (Json::Value &_return, Json::Value &params)
{
  try {
    if (params.empty() ) {
      _return ["error"] =
        "'create' method requires params with at least property 'type'";
    } else if (!params["type"].isString() ) {
      _return ["error"] = "'type' is not a string";
    } else if (params["type"].asString() == "MediaPipeline") {
      std::shared_ptr<MediaPipeline> mediaPipeline;

      mediaPipeline = std::shared_ptr<MediaPipeline> (new MediaPipeline (modules,
                      mediaSet) );
      mediaSet.reg (mediaPipeline);

      _return["result"]["id"] = mediaPipeline->getIdStr();
    } else {
      _return ["error"] = "'type' " + params["type"].asString() + " is not supported";
    }

  } catch (...) {
    _return["error"] = "Unexpected error in create";
  }
}

void
MediaServerServiceHandler::invokeJsonRpc (std::string &_return,
    const std::string &request)
{
  Json::Reader reader;
  Json::FastWriter writer;
  Json::Value id;

  try {
    Json::Value jsonRequest;
    Json::Value response;

    reader.parse (request, jsonRequest);

    GST_INFO ("JsonRpc request: %s", request.c_str() );

    id = jsonRequest ["id"];
    response["id"] = id;
    response["error"];
    response["result"];
    response["jsonrpc"] = "2.0";

    if (jsonRequest.empty() ) {
      response ["error"] = "Error on request, empty or not parseable";
    } else if (!jsonRequest["method"].isString() ) {
      response ["error"] = "'method' is not a string";
    } else if (jsonRequest["method"].asString() == "create") {
      create (response, jsonRequest["params"]);
    } else {
      response ["error"] = "Method not supported";
      response ["result"] = "Method not supported";
    }

    _return = writer.write (response);
    GST_INFO ("JsonRpc return: %s", _return.c_str() );
  } catch (...) {
    Json::Value response;

    response["result"] = "Unexpected exception";
    response["error"] = "Unexpected exception";
    response["jsonrpc"] = "2.0";
    response["id"] = id;

    GST_ERROR ("Unexpected exception");
    _return = writer.write (response);
  }
}


MediaServerServiceHandler::StaticConstructor
MediaServerServiceHandler::staticConstructor;

MediaServerServiceHandler::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} // kurento
