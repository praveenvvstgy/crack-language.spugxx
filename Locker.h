/*===========================================================================*\

   $Id$

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

#ifndef SPUG_LOCKER_H
#define SPUG_LOCKER_H

#include "Mutex.h"
#include <stdlib.h>

namespace spug {

/**
 * Locks a mutex when constructed, releases when destroyed.
 */
class Locker {
   private:
      Mutex &mutex;

   public:
      Locker(Mutex &mutex) : mutex(mutex) {
         mutex.acquire();
      }

      ~Locker() {
         try {
            mutex.release();
         } catch (...) {
            abort();
         }
      }
};

} // namespace spug

#endif
