// PtrSList
//    Singly linked list class for lists of ObjPtr's

#ifndef PtrSList_H
#define PtrSList_H

#ifdef __GNUG__
#pragma interface
#endif

#ifndef Ptrs_H
#include "Ptrs.h"
#endif

#ifndef PSLBase_H
#include "PSLBase.h"
#endif

template <class T>
class PtrSList : public PtrSListBase {

   private:

		class PtrNode;
		friend class PtrNode;
      class PtrNode : public Node {
         public:
            ObjPtr<T> ptr;
            inline PtrNode(ObjPtr<T> ptr0) : ptr(ptr0) {}
            inline PtrNode(T *ptr0) : ptr(ptr0) {}
         };

      static void freeNode(Node *node)
         {
         delete (PtrNode*)node;
         }

      PtrSList(const PtrSList<T> &other);

   public:
      inline PtrSList()
         {
         }
         
      inline PtrSList(PtrSList<T> &other)
         {
         ripOff(other);
         }
               
      inline ~PtrSList()
         {
         PtrSListBase::clear(freeNode);
         }
         
      inline PtrSList<T> &operator =(PtrSList<T> &other)
         {
         PtrSListBase::clear(freeNode);
         ripOff(other);
			return *this;
         }

      inline Pix prepend(T *item)
         {
         return PtrSListBase::prepend(new PtrNode(item));
         }

      inline Pix prepend(ObjPtr<T> &item)
         {
         return PtrSListBase::prepend(new PtrNode(item));
         }

      inline Pix append(T *item)
         {
         return PtrSListBase::append(new PtrNode(item));
         }

      inline Pix append(ObjPtr<T> &item)
         {
         return PtrSListBase::append(new PtrNode(item));
         }

      inline ObjPtr<T> &operator ()(Pix p)
         {
         return ((PtrNode*)p)->ptr;
         }
         
      inline const ObjPtr<T> &operator ()(Pix p) const
         {
         return ((PtrNode*)p)->ptr;
         }
         
      inline ObjPtr<T> &front()
         {
         return operator ()(first());
         }

      inline const ObjPtr<T> &front() const
         {
         return operator ()(first());
         }
         
      inline ObjPtr<T> &rear()
         {
         return operator ()(last());
         }
         
      inline const ObjPtr<T> &rear() const
         {
         return operator ()(last());
         }
      
      // removes node referenced by 'p' and returns Pix to next node or NULL
      inline Pix remove(Pix p)
         {
         return PtrSListBase::remove(p, freeNode);
         }
         
      inline Pix reset(Pix p, T *newItem)
         {
         ObjPtr<T> newItemPtr(newItem);
         ((PtrNode*)p)->ptr = newItemPtr;
         return p;
         }

      inline Pix reset(Pix p, ObjPtr<T> &newItem)
         {
         ((PtrNode*)p)->ptr = newItem;
         return p;
         }
         
      inline void clear()
         {
         PtrSListBase::clear(freeNode);
         }

   };

#endif


