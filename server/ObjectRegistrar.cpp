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

#include "ObjectRegistrar.hpp"

namespace kurento
{

void
ObjectRegistrar::registerFactory (std::shared_ptr<Factory> factory)
{
  std::string factoryName = factory->getName();

  if (factories.find (factoryName) != factories.end() ) {
    std::cerr << "Module " <<  factoryName << " already registered." << std::endl;
    return;
  }

  factories[factoryName] = factory;
}

std::shared_ptr<Factory>
ObjectRegistrar::getFactory (const std::string &factoryName)
{
  if (factories.find (factoryName) != factories.end() ) {
    return factories[factoryName];
  }

  std::cerr << "Module " <<  factoryName << " not found." << std::endl;
  return std::shared_ptr<Factory> ();
}

std::shared_ptr<ObjectRegistrar> objectRegistrar (new ObjectRegistrar() );

} /* kurento */
