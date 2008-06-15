/*===========================================================================*\
     
    Interface & implementation of Iterator.h

    Copyright (C) 2006 Michael A. Muller
 
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
