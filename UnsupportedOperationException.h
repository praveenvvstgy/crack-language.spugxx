/*===========================================================================*\

   $Id$

   Copyright (C) 2007 Michael A. Muller

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
#ifndef SPUG_UNSUPPORTEDOPERATIONEXCEPTION_H
#define SPUG_UNSUPPORTEDOPERATIONEXCEPTION_H

#include "Exception.h"

namespace spug {

/**
 * This can be thrown any time that a function must be implemented, but is not
 * appropriate for a particular implementation.
 *
 * For example, if the class Circle derives from Shape, and Shape defines an
 * abstract "getLongestSide()" method, it would be appropriate for Circle's
 * implementation of this method to always throw
 * UnsupportedOperationException.
 */
SPUG_EXCEPTION(UnsupportedOperationException);

} // namespace spug

#endif
