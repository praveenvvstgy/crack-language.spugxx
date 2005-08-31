/*===========================================================================*\

   RCBase.h - reference counted object base class

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

#ifndef SPUG_RCBASE_H
#define SPUG_RCBASE_H

namespace spug {

/** 
 * Reference counting base class.  This class is not thread-safe.
 */
class RCBase {

   private:
      int refCount;

   public:
      RCBase() : refCount(0) {}
      virtual ~RCBase() {}

      /** increment the reference count */
      void incref() { ++refCount; }

      /** decrement the reference count */
      void decref() { if (!--refCount) delete this; }

      /** return the reference count */
      int refcnt() const { return refCount; }
};

}

#endif
