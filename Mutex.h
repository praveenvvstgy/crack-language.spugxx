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

#ifndef SPUG_MUTEX_H
#define SPUG_MUTEX_H

#include <pthread.h>

namespace spug {

class Condition;

/** Mutual exclusion lock */
class Mutex {
   private:
      pthread_mutex_t mutex;

      Mutex(const Mutex &other);

   public:
      friend class Condition;

      /**
       * @param recursive create a recursive mutex - one that can be locked
       *                  multiple times within the same thread
       */
      Mutex(bool recursive = true);

      ~Mutex();

      /** Acquire the mutex, wait for it if it is already locked.  */
      void acquire();

      /** Release the mutex. */
      void release();
};

} // namespace spug

#endif
