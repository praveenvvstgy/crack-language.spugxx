/*===========================================================================*\

   RCPtr.h - reference counted pointer template.


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

#ifndef SPUG_RCPTR_H
#define SPUG_RCPTR_H

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

      /** trickery to allow us to convert to a base class pointer */
      template <class U>
      operator const RCPtr<U> &() {
	 // let the compiler verify that T derives from U (and hopefully
	 // optimize this away)
	 U *dummy = obj;
	 return *(reinterpret_cast<const RCPtr<U> *>(this));
      }

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
      operator int() {
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

#endif

