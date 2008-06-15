/*===========================================================================*\

    Basic, non-class, type definitions.
     
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

#ifndef SPUG_TYPES_H
#define SPUG_TYPES_H

namespace spug {

namespace Const {
   enum {
      // buffer big enough for the output of strerror_r
      errorBufferSize = 256
   };
}

typedef unsigned char Byte;
typedef short Int16;

}

#endif
