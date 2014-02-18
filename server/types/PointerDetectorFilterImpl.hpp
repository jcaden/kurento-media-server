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

#ifndef __POINTER_DETECTOR_FILTER_IMPL_HPP__
#define __POINTER_DETECTOR_FILTER_IMPL_HPP__

#include "FilterImpl.hpp"
#include <generated/PointerDetectorFilter.hpp>

namespace kurento
{

class PointerDetectorFilterImpl: public virtual PointerDetectorFilter,
  public FilterImpl
{
public:
  PointerDetectorFilterImpl (std::shared_ptr<PointerDetectorWindowMediaParam>
                             window, std::shared_ptr<MediaObjectImpl> parent, int garbagePeriod);
  virtual ~PointerDetectorFilterImpl() throw () {};

  void addWindow (std::shared_ptr<PointerDetectorWindowMediaParam> window);
  void clearWindows ();
  void removeWindow (const std::string &windowId);

private:

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;

};

} /* kurento */

#endif /* __POINTER_DETECTOR_FILTER_IMPL_HPP__ */
