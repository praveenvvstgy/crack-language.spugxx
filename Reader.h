/*===========================================================================*\
     
    Reader interface.

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

#ifndef SPUG_READER_H
#define SPUG_READER_H

#include <sys/types.h>
#include "types.h"

namespace spug {

/**
   Reader interface - base class of everything that can read a sequence of
   bytes.
 */
class Reader {

   public:
      /**
       * Reads 'count' bytes into buffer, or as many bytes are available,
       * depending on the nature of the implementation.
       */
      virtual size_t read(Byte *buffer, size_t bytes) = 0;
};

} // namespace spug

#endif
