/*===========================================================================*\

    LPtr.h - lightweight reference counted pointer template.
     
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
	 // increment the new object, release the existing object.  The order is 
	 // important, as the old object could reference the new one.
	 if (obj0) obj0->incref();
	 if (obj) obj->decref();

	 // link to the new one
	 obj = obj0;
	 return *this;
      }
      
      LPtr<T> &operator =(LPtr<T> &other) {
         return *this = other.obj;
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

