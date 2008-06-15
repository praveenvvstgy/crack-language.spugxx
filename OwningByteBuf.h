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
