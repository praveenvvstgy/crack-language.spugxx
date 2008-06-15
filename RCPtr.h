/*===========================================================================*\
     
    RCPtr.h - reference counted pointer template.

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

#ifndef SPUG_RCPTR_H
#define SPUG_RCPTR_H

#include <iostream>
#include <exception>
#include <assert.h>

namespace spug {

/**
 * Another intrusive reference-counted pointer class.  This class is all about
 * convenience - users are free to access the underlying pointer if they want
 * to.
 *
 * This is meant to be used to manage an instance of RCBase.  It can be used
 * with any class that implements incref() and decref().
 */
template <class T>
class RCPtr {
   public:

      // The raw pointer
      T *obj;

      /** Copy constructor. */
      RCPtr(const RCPtr<T> &other) : obj(other.obj) {
	 if (obj) obj->incref();
      }

      /** Constructs a RCPtr from a T*. */
      RCPtr(T *obj0) : obj(obj0) {
	 if (obj) obj->incref();
      }

      /** Construct from a derived class RCPtr */
      template <class U>
      RCPtr(const RCPtr<U> &other) : obj(0) {
	 if (other.obj) {
	    obj = dynamic_cast<T *>(other.obj);
	    if (!obj)
	       throw std::bad_cast();
	    else
	       obj->incref();
	 }
      }
	 

      /** Constructs a RCPtr initialized to NULL. */
      RCPtr() : obj(0) {}

      ~RCPtr() {
	 if (obj) obj->decref();
      }

      /** Copies another *RCPtrBase* to the receiver. */
      RCPtr<T> &operator =(const RCPtr<T> &other) {
	 *this = other.obj;
      }

      /** Assigns a T* to the receiver. */
      RCPtr<T> &operator =(T *obj0) {
	 // release the existing object
	 if (obj) obj->decref();

	 // link to the new one
	 obj = obj0;
	 if (obj0) obj0->incref();
      }


      /** 
       * Upcast an pointer to a derived class U to the base class pointer.
       * This will fail at compile time if U is not derived from T.
       */
      template <class U>
      static const RCPtr<T> &ucast(const RCPtr<U> &other) {
	 // let the compiler verify that T derives from U (and hopefully
	 // optimize this away)
	 T *dummy = other.obj;
	 assert(reinterpret_cast<void *>(dummy) == 
	         reinterpret_cast<void *>(other.obj)
		);
	 return *(reinterpret_cast<const RCPtr<T> *>(&other));
      }

      /**
       * Downcast a pointer to a base class U to the derived class pointer.
       * This will fail at runtime with a "std::bad_cast" exception if the
       * referenced object is not a T instance.
       */
      template <class U>
      static const RCPtr<T> &dcast(const RCPtr<U> &other) {
	 // again, let the system verify using a dynamic cast
	 dynamic_cast<T&>(*other.obj);
	 return *(reinterpret_cast<const RCPtr<T> *>(&other));
      }

#if 0
      /** 
       * trickery to allow us to convert to a base class pointer.  This
       * doesn't seem to work on some compilers, and doesn't work well on
       * almost any - pray you avoid it.  Stick to the explict casts.
       */
      template <class U>
      operator const RCPtr<U> &() {
	 // let the compiler verify that T derives from U (and hopefully
	 // optimize this away)
	 U *dummy = obj;
	 return *(reinterpret_cast<const RCPtr<U> *>(this));
      }
#endif

      /**
       * Used to invoke a member function on the receiver's *ManagedObject*
       */
      T *operator ->() const {
	 return obj;
      }

      /** Used to deal directly with the receiver's member object. */
      T &operator *() const {
	 return *obj;
      }

      /** allows us to easily check for NULL in a conditional statement. */
      operator int() const {
	 return (int)obj;
      }
      
      /**
         Allows us to compare the pointer value of two Managed Object
         Pointers.
       */
      int operator ==(const RCPtr<T> &other) const {
	 return obj == other.obj;
      }
         
      /**
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
       */
      int operator ==(const void *ptr) const {
	 return (void*)obj == ptr;
      }

      /**
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
       */
      friend int operator ==(const void *ptr1, const RCPtr<T> &ptr2) {
	 return (void*)ptr2.obj == ptr1;
      }

      /**
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
       */
      int operator ==(int ptr) const {
	 return (int)obj == ptr;
      }

      /**
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
       */
      friend int operator ==(int ptr1, const RCPtr<T> &ptr2) {
	 return (int)ptr2.obj == ptr1;
      }

   };

} // namespace spug

/**
  macro to easily define RCPtr instances, complete with a forward
  declaration.
*/
#define SPUG_RCPTR(cls) class cls; typedef spug::RCPtr<cls> cls##Ptr;

#endif

