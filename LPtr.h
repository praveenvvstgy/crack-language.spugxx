/*===========================================================================*\

   LPtr.h - lightweight reference counted pointer template.


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

#ifndef SPUG_LPTR_H
#define SPUG_LPTR_H

#include <iostream>
#include <exception>
#include <assert.h>

namespace spug {

/**
 * This is a variation on RCPtr that simply dispenses with the "pointer
 * trapping" semantics - instead, it features automatic conversion to the raw
 * pointer type, which I think will solve a lot of problems.
 */
template <class T>
class LPtr {
   private:
      // The raw pointer
      T *obj;

   public:

      /** Copy constructor. */
      LPtr(const LPtr<T> &other) : obj(other.obj) {
	 if (obj) obj->incref();
      }

      /** Constructs a RCPtr from a T*. */
      LPtr(T *obj0) : obj(obj0) {
	 if (obj) obj->incref();
      }

      /** Constructs a LPtr initialized to NULL. */
      LPtr() : obj(0) {}

      ~LPtr() {
	 if (obj) obj->decref();
      }

      /** Implicit cast to the raw pointer type. */
      operator T *() { return obj; }

      /** Assigns a T* to the receiver. */
      LPtr<T> &operator =(T *obj0) {
	 // release the existing object
	 if (obj) obj->decref();

	 // link to the new one
	 obj = obj0;
	 if (obj0) obj0->incref();
      }

      /**
       * Used to invoke a member function on the receiver's *ManagedObject*
       */
      T *operator ->() const {
	 return obj;
      }
   };

} // namespace spug

/**
  macro to easily define LPtr instances, complete with a forward
  declaration.
*/
#define SPUG_LPTR(cls) class cls; typedef spug::LPtr<cls> cls##LPtr;

#endif

