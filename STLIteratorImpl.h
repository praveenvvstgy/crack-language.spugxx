/*===========================================================================*\
    
    Interface & implementation of STLIteratorImpl template
    
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

#ifndef SPUG_STLITERATORIMPL_H
#define SPUG_STLITERATORIMPL_H

#include "IteratorImpl.h"

namespace spug {

/**
 * Iterator implementation based on STL iterators.  See Iterator.
 */
template <class Elem, typename Collection, 
          typename iterator = typename Collection::iterator>
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

      virtual const Elem &cur() {
	 return *begin;
      }

      virtual bool isValid() const {
	 return begin != end;
      }

};

} // namespace spug

#endif
