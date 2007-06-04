/*===========================================================================*\

   Copyright (C) 2007 Michael A. Muller

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
