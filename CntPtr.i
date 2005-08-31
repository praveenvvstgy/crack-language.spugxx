
//#BEGINDOC /\*-+\*\\
//#ENDDOC \\\*-+\*/

#ifndef CntPtr_I
#define CntPtr_I

//#pragma implementation

#ifndef CntPtrBase_H
#include "CntPtrBase.h"
#endif

template <class T>
inline CntPtr<T>::CntPtr(const CntPtr<T> &other) :
		CntPtrBase( other.obj )
	{
	}

template <class T>
inline CntPtr<T>::CntPtr(T *obj0) : CntPtrBase(obj0)
	{
	}

template <class T>
inline CntPtr<T>::CntPtr()
	{
	}

template <class T>
inline CntPtr<T> &CntPtr<T>::operator =(const CntPtr<T> &other)
   {
	assign(other.obj);
	return *this;
	}

template <class T>
inline CntPtr<T> &CntPtr<T>::operator =(T *obj0)
   {
	assign(obj0);
	return *this;
	}

template <class T>
inline T *CntPtr<T>::operator ->() const
	{
	return (T*)obj;
	}

template <class T>
inline T &CntPtr<T>::operator *() const
	{
	return *(T*)obj;
	}

template <class T>
inline CntPtr<T>::operator int()
	{
	return (int)obj;
	}

template <class T>
inline int CntPtr<T>::operator ==(const CntPtr<T> &other) const
   {
	return obj == other.obj;
	}

template <class T>
inline int CntPtr<T>::operator ==(const void *ptr) const
   {
	return (void*)obj == ptr;
	}

template <class T>
inline int operator ==(const void *ptr1, const CntPtr<T> &ptr2)
   {
	return (void*)ptr2.obj == ptr1;
	}

template <class T>
inline int CntPtr<T>::operator ==(int ptr) const
   {
	return (int)obj == ptr;
	}

template <class T>
int operator ==(int ptr1, const CntPtr<T> &ptr2)
   {
	return (int)ptr2.obj == ptr1;
	}

#endif

