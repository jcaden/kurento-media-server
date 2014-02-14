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

#ifndef __MEDIA_PIPELINE_HPP__
#define __MEDIA_PIPELINE_HPP__

#include "MediaObjectParent.hpp"
#include "MediaHandler.hpp"
#include <common/MediaSet.hpp>
#include <module.hpp>

namespace kurento
{

class MediaElement;
class Mixer;

class MediaPipeline : public MediaObjectParent,
//   public KmsMediaPipeline,
public std::enable_shared_from_this<MediaPipeline>
{

public:
  MediaPipeline (std::map<std::string, KurentoModule *> &modules,
                 MediaSet &mediaSet) ;
  ~MediaPipeline() throw();

  std::shared_ptr<MediaPipeline> getPipeline() {
    return shared_from_this();
  };

  GstElement *pipeline;

private:
  std::map <std::string, KurentoModule *> &modules;

  void init ();

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;

  friend void media_pipeline_receive_message (GstBus *bus, GstMessage *message,
      gpointer data);
};

} // kurento

#endif /* __MEDIA_PIPELINE_HPP__ */
