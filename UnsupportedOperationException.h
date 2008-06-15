/*===========================================================================*\
     
    Copyright (C) 2007 Michael A. Muller
 
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
