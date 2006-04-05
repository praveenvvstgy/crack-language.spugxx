/*===========================================================================*\

   Interface for Tracer class.

   Copyright (C) 2005 Michael A. Muller

   Permission is granted to use, modify and redistribute this code,
   providing that the following conditions are met:
 
   1) This copyright/licensing notice must remain intact.
   2) If the code is modified and redistributed, the modifications must 
   include documentation indicating that the code has been modified.
   3) The author(s) of this code must be indemnified and held harmless
   against any damage resulting from the use of this code.

   This code comes with ABSOLUTELY NO WARRANTEE, not even the implied 
   warrantee of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

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
