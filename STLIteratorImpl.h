/*===========================================================================*\

   $Id$

   Interface & implementation of STLIteratorImpl template

   Copyright (C) 2006 Michael A. Muller

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

#ifndef SPUG_STLITERATORIMPL_H
#define SPUG_STLITERATORIMPL_H

#include "IteratorImpl.h"

namespace spug {

/**
 * Iterator implementation based on STL iterators.  See Iterator.
 */
template <class Elem, typename Collection, typename iterator>
class STLIteratorImpl : public IteratorImpl<Elem> {

   public:
      //typedef STLIter iterator;
      //typedef Collection::iterator iterator;
   private:

      typedef STLIteratorImpl<Elem, Collection, iterator> Type;

      iterator begin, end;
      bool allocated;

      // define operator new as private to force the use of alloc
      void *operator new(size_t size) {
	 return ::operator new(size);
      }

   public:

      STLIteratorImpl(iterator begin, iterator end) : 
	 begin(begin), 
	 end(end),
	 allocated(false) {
      }

      STLIteratorImpl(Collection &coll) : 
	 begin(coll.begin()), 
	 end(coll.end()) {
      }

      static Type *alloc(iterator begin, iterator end) {
	 Type *impl = new Type(begin, end);
	 impl->allocated = true;
	 return impl;
      }

      static Type *alloc(Collection &coll) {
	 Type *impl = new Type(coll);
	 impl->allocated = true;
	 return impl;
      }

      virtual void release() {
	 if (allocated)
	    delete this;
      }

      virtual IteratorImpl<Elem> *copy() {
	 Type *impl = new STLIteratorImpl<Elem, Collection, iterator>(*this);
	 impl->allocated = true;
	 return impl;
      }

      virtual void next() {
	 begin++;
      }

      virtual Elem &cur() {
	 return *begin;
      }

      virtual bool isValid() const {
	 return begin != end;
      }

};

} // namespace spug

#endif
