/*===========================================================================*\
    
    Tracer class implementation.
    
    Copyright (C) 2005 Michael A. Muller
 
    This file is part of spug++.
 
    spug++ is free software: you can redistribute it and/or modify it under the 
    terms of the GNU Lesser General Public License as published by the Free 
    Software Foundation, either version 3 of the License, or (at your option) 
    any later version.
 
    spug++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public License 
    along with spug++.  If not, see <http://www.gnu.org/licenses/>.
 
\*===========================================================================*/

#include "Tracer.h"
#include <string>
#include <iostream>

using namespace spug;
using namespace std;

// the tracer singleton
Tracer *Tracer::theTracer = 0;

Tracer::Tracer() : lastLevel(0) {
}

// the only reason that this isn't inlined is to guarantee that "theTracer"
// has been initialized.
Tracer &Tracer::get() {
   if (!theTracer)
      theTracer = new Tracer();

   return *theTracer;
}

Tracer::Level Tracer::getLevel(const char *levelName) {
   string name = levelName;

   // if the name is in the map, return the existing value, else create a new
   // value.
   NameMap::iterator cur = levelByName.find(levelName);
   if (cur != levelByName.end())
      return cur->second;
   else {
      // increase the capacity of the vector
      levels.push_back(false);
      return levelByName[name] = lastLevel++;
   }
}
