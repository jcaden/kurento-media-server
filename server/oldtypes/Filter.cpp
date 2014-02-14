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

#include "Filter.hpp"

#define FACTORY_NAME "filterelement"

namespace kurento
{

Filter::Filter (MediaSet &mediaSet, std::shared_ptr<MediaObjectImpl> parent,
                const std::string &filterType)
  : MediaElement (mediaSet, parent, filterType, FACTORY_NAME)
{
}

Filter::~Filter() throw ()
{

}

} // kurento
