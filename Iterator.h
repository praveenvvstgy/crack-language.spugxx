/*===========================================================================*\

   $Id$

   Interface & implementation of Iterator.h

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

#ifndef SPUG_ITERATOR_H
#define SPUG_ITERATOR_H

#include "Iterator.h"
#include "IteratorImpl.h"
#include <assert.h>

namespace spug {

/**
 * An alternative to STL style iterators.  This kind of iterator is
 * self-contained, so you don't have to pass around the end pointer and you
 * don't  have to have knowledge of the container type to use it.  This makes
 * it suitable for use in interfaces.
 */
template <class Elem>
class Iterator {
   private:
      IteratorImpl<Elem> *impl;

   public:

      Iterator() : impl(0) {}

      Iterator(IteratorImpl<Elem> *impl) : impl(impl) {}

      Iterator(const Iterator &other) : impl(other.impl->copy()) {}

      Iterator<Elem> &operator =(const Iterator<Elem> &other) {
	 if (impl)
	    impl->release();
	 impl = other.impl->copy();
      }

      ~Iterator() {
	 if (impl)
	    impl->release();
      }

      /**
       * Dereferences the iterator.
       */
      const Elem &operator *() {
	 assert(impl);
	 return impl->cur();
      }

      /**
       * Dereferences members of the iterator.
       */
      const Elem *operator ->() {
         assert(impl);
         return &impl->cur();
      }

      /**
       * Post-increment operator - increments the iterator and returns the
       * original value.
       */
      Iterator operator ++(int) {
	 assert(impl);
         Iterator temp(impl);
	 impl->next();
	 return temp;
      }

      /**
       * Pre-increment operator - increments the iterator and returns the
       * new value.
       */
      Iterator &operator ++() {
	 assert(impl);
	 impl->next();
	 return *this;
      }

      /** 
       * Converts the iterator to boolean so we can say "if (iter)" to check
       * for validity.
       */
      operator bool() const {
	 assert(impl);
	 return impl->isValid();
      }
};

} // namespace spug

#endif
