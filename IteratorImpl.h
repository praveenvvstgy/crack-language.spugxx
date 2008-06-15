/*===========================================================================*\

    Interface & implementation for IteratorImpl.
     
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

#ifndef SPUG_ITERATORIMPL_H
#define SPUG_ITERATORIMPL_H

namespace spug {

/**
 * Abstract Iterator implementation base class.  Classes wishing to implement
 * an interator should derive from this.
 */
template <class Elem>
class IteratorImpl {

   public:
      virtual ~IteratorImpl() {}

      /**
       * Called by an Iterator to signal that it is done using the
       * implementation.  The iterator implementation should do whatever
       * cleanup is appropriate, including deleting itself if necessary.
       */
      virtual void release() = 0;

      /**
       * Returns a copy of the iterator implementation for a duplicate
       * Iterator.  This doesn't actually have to be a copy - you can share
       * the sample implementation among multiple Iterators as long as the
       * release() method can handle multiple releases.
       */
      virtual IteratorImpl<Elem> *copy() = 0;

      /**
       * Moves the iterator forward one element.
       */
      virtual void next() = 0;

      /**
       * Returns a reference to the current item.
       */
      virtual const Elem &cur() = 0;

      /**
       * Returns true if the iterator is valid, false if not.
       */
      virtual bool isValid() const = 0;

};

} // namespace spug

#endif
