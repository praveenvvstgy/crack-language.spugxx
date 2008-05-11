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
