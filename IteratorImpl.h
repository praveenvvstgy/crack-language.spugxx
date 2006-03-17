/*===========================================================================*\

   $Id$

   Interface & implementation for IteratorImpl.

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
      virtual Elem &cur() = 0;

      /**
       * Returns true if the iterator is valid, false if not.
       */
      virtual bool isValid() const = 0;

};

} // namespace spug

#endif
