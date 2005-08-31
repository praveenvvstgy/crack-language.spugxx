
//#BEGINDOC /\*-+\*\\
//#ENDDOC \\\*-+\*/

#ifndef CntPtr_H
#define CntPtr_H

//#pragma interface

#ifndef CntPtrBase_H
#include "CntPtrBase.h"
#endif

/*--------------------------------------------------------------------------*\
   The *CntPtrBase* template class.   Provides type safety on top of
   the reference counting mechanism.
   *CntPtrBase* is designed to be as leak proof as possible: it does
   not define the T* operator which would allow a careless programmer to
   do this:
   {{
      PtrTo<SomeClass> p =
         getSomeClassPtr();
      SomeClass *q = p;                // reference count not incremented!
   }}
   However, the safety mecahnisms are not foolproof.  A programmer could
   still leak a pointer to a non-reference counter pointer by using the
   raw output of the '*' and '->' operators:
   {{
      PtrTo<SomeClass> p =
         getSomeClassPtr();
      SomeClass *q = p.operator ->();
      SomeClass *r = &p.operator *();
   }}
   *Don't*do*this.*  Also, avoid storing a /reference/ to a
   *CntPtrBase* in an instance variable (it's OK to use references to
   these in parameter lists.  In fact, it's even recommended as this saves
   time on needlessly incrementing and decrementing a local variable).
\*--------------------------------------------------------------------------*/
template <class T>
class CntPtr : public CntPtrBase {
   public:

      /*--------------------------------------------------------------------*\
         Copy constructor.
      \*--------------------------------------------------------------------*/
      CntPtr(const CntPtr<T> &other) ;/**:
         	CntPtrBase( other.obj )
			{
			}**/

      /*--------------------------------------------------------------------*\
         Constructs a *CntPtrBase* from a T*.
      \*--------------------------------------------------------------------*/
      CntPtr(T *obj0);/** : CntPtrBase(obj0)
			{
			}**/

      /*--------------------------------------------------------------------*\
         Constructs a *MangedObjectPtr* initialized to NULL.
      \*--------------------------------------------------------------------*/
      CntPtr();/**
			{
			}**/

      /*--------------------------------------------------------------------*\
         Copies another *CntPtrBase* to the receiver.
      \*--------------------------------------------------------------------*/
      CntPtr<T> &operator =(const CntPtr<T> &other);/**
         {
			assign(other.obj);
			return *this;
			}**/

      /*--------------------------------------------------------------------*\
         Assigns a T* to the receiver.
      \*--------------------------------------------------------------------*/
      CntPtr<T> &operator =(T *obj0);/**
         {
			assign(obj0);
			return *this;
			}**/

      /*--------------------------------------------------------------------*\
         Used to invoke a member function on the receiver's *ManagedObject*
      \*--------------------------------------------------------------------*/
      T *operator ->() const;/**
			{
			return (T*)obj;
			}**/

      /*--------------------------------------------------------------------*\
         Used to deal directly with the receiver's member object.
      \*--------------------------------------------------------------------*/
      T &operator *() const;/**
			{
			return *(T*)obj;
			}**/

      /*--------------------------------------------------------------------*\
         allows us to easily check for NULL in a conditional statement.
      \*--------------------------------------------------------------------*/
      operator int();/**
			{
			return (int)obj;
			}**/
      
      /*---------------------------------------------------------------------*\
         Allows us to compare the pointer value of two Managed Object
         Pointers.
      \*---------------------------------------------------------------------*/
      int operator ==(const CntPtr<T> &other) const;/**
         {
			return obj == other.obj;
			}**/
         
      /*---------------------------------------------------------------------*\
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
      \*---------------------------------------------------------------------*/
      int operator ==(const void *ptr) const;/**
         {
			return (void*)obj == ptr;
			}**/

      /*---------------------------------------------------------------------*\
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
      \*---------------------------------------------------------------------*/
      friend int operator ==(const void *ptr1, const CntPtr<T> &ptr2);/**
         {
			return (void*)ptr2.obj == ptr1;
			}**/

      /*---------------------------------------------------------------------*\
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
      \*---------------------------------------------------------------------*/
      int operator ==(int ptr) const;/**
         {
			return (int)obj == ptr;
			}**/

      /*---------------------------------------------------------------------*\
         Allows us to compare the pointer value to any kind of pointer.
         Basically, this exists to permit comparison to NULL.
      \*---------------------------------------------------------------------*/
      friend int operator ==(int ptr1, const CntPtr<T> &ptr2);/**
         {
			return (int)ptr2.obj == ptr1;
			}**/

   };


#endif

