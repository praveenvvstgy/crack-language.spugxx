/*===========================================================================*\
     
    Copyright (C) 2007 Michael A. Muller
 
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

#ifndef SPUG_LOCKER_H
#define SPUG_LOCKER_H

#include "Mutex.h"
#include <stdlib.h>

namespace spug {

/**
 * Locks a mutex when constructed, releases when destroyed.
 */
template<class T>
class GenericLocker {
   private:
      T &mutex;

   public:
      GenericLocker(T &mutex) : mutex(mutex) {
         mutex.acquire();
      }

      ~GenericLocker() {
         try {
            mutex.release();
         } catch (...) {
            abort();
         }
      }
};

class Condition;

typedef GenericLocker<Mutex> Locker;
typedef GenericLocker<Condition> ConditionLocker;

} // namespace spug

#endif
