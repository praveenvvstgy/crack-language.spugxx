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

#ifndef SPUG_OWNINGBYTEBUF_H
#define SPUG_OWNINGBYTEBUF_H

#include "ByteBuf.h"

namespace spug {

/**
 * A ByteBuf that maintains ownership over its buffer, freeing it during 
 * destruction.
 */
class OwningByteBuf : public ByteBuf {
    private:

        /** Allocate memory and copy a buffer. */
        Byte *dup(size_t size, const Byte *data);

        /**
         * Non-const buffer constructor - transfers ownership of an existing 
         * buffer.  We hide this in the wrap() method to protect the user from 
         * accidentally binding a non-const buffer.
         * 
         * The last argument is just to disambiguate this from the const 
         * method.
         */
        OwningByteBuf(size_t size, Byte *buffer, bool) : 
            ByteBuf(size, buffer) {
        }

    public:
        
        /**
         * Construct from a buffer size and a raw buffer.  The raw buffer data 
         * will be copied - it will remain the responsibility of the caller to 
         * manage its memory.
         */
        OwningByteBuf(size_t size, const Byte *buffer) :
            ByteBuf(size, dup(size, buffer)) {
        }
        
        OwningByteBuf(const ByteBuf &other) :
            ByteBuf(other.size, dup(other.size, other.buffer)) {
        }
        
        ~OwningByteBuf() {
            delete buffer;
        }

        /**
         * Construct an owning byte buffer, transfering ownership of an 
         * existing raw buffer.
         * @param size buffer size in bytes
         * @param buffer raw buffer (will be owned by the owning buffer and 
         *             deleted during destruction.
         */
        static OwningByteBuf wrap(size_t size, Byte *buffer) {
            return OwningByteBuf(size, buffer, true);
        }
};

} // namespace spug

#endif
