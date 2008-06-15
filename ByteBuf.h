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

#ifndef SPUG_BYTEBUF_H
#define SPUG_BYTEBUF_H

#include <sys/types.h>
#include "types.h"

namespace spug {

/**
 * A byte buffer - this is a simple aggregation of a buffer pointer and its 
 * size.  It does not manager the buffer.  For a derived class that _does_ 
 * manage the buffer, see OwningByteBuf.
 */
struct ByteBuf {
    size_t size;
    Byte *buffer;

    /**
     * Creates the ByteBuf from it size and buffer
     * @param size size of the buffer in bytes.
     * @param buffer The buffer pointer
     */
    ByteBuf(size_t size, Byte *buffer) : size(size), buffer(buffer) {}
    
    /** Default constructor */
    ByteBuf() : size(0), buffer(0) {}

};

} // namespace spug

#endif
