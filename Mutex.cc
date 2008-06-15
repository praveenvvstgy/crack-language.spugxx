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

#include "Mutex.h"

#include <stdlib.h>
#include <errno.h>

#include "SystemException.h"

using namespace spug;

Mutex::Mutex(bool recursive) {
   if (recursive) {
      pthread_mutexattr_t attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

      pthread_mutex_init(&mutex, &attr);
   } else {
      pthread_mutex_init(&mutex, 0);
   }
}

Mutex::~Mutex() {
   if (pthread_mutex_destroy(&mutex))
      abort();
}

void Mutex::acquire() {
   if (pthread_mutex_lock(&mutex))
      throw SystemException(errno, "locking mutex");
}

void Mutex::release() {
   if (pthread_mutex_unlock(&mutex))
      throw SystemException(errno, "unlocking mutex");
}
