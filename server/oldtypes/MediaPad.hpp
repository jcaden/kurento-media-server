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

#ifndef __MEDIA_PAD_HPP__
#define __MEDIA_PAD_HPP__

#include "MediaObjectImpl.hpp"

namespace kurento
{

class MediaElement;

struct KmsMediaType {
  enum type {
    AUDIO = 0,
    VIDEO = 1,
    DATA = 2
  };
};

class MediaPad : public MediaObjectImpl/*,
  public KmsMediaPad*/
{
public:
  MediaPad (std::shared_ptr<MediaElement> parent, KmsMediaType::type mediaType);
  MediaPad (std::shared_ptr<MediaElement> parent, KmsMediaType::type mediaType,
            const std::string mediaDescription);
  virtual ~MediaPad () throw () = 0;

  std::shared_ptr<MediaElement> getMediaElement ();

  std::shared_ptr<MediaPipeline> getPipeline () {
    return parent->getPipeline();
  }

protected:
  GstElement *getElement ();
  KmsMediaType::type mediaType;

private:
  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;
};

} // kurento

#endif /* __MEDIA_PAD_HPP__ */
