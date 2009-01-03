/*===========================================================================*\
    
    Reactable interface.
    
    Copyright (C) 2008 Michael A. Muller
 
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

#ifndef SPUG_REACTABLE_H
#define SPUG_REACTABLE_H

#include "LPtr.h"
#include "RCBase.h"

namespace spug {

class Reactor;

/**
 * Interface for objects that can be managed with a Reactor.
 */
class Reactable : public virtual RCBase {
    public:
        typedef enum { readyToRead = 1, readyToWrite = 2 } Status;
        
        /**
         * Returns the status of the reactable - this is some combination of 
         * readyToRead and readyToWrite or 0 if neither is the case.
         */
        virtual Status getStatus() = 0;
        
        /**
         * Called when the reactor detects that the reactable is ready to read.
         * The implementation should perform a read on the underlying file 
         * descriptor.
         */
        virtual void handleRead(Reactor &reactor) = 0;
        
        /**
         * Called when the reactor detects that the reactable is ready to 
         * write.  The implementation should perform a write on the underlying 
         * file descriptor.
         */
        virtual void handleWrite(Reactor &reactor) = 0;
        
        /**
         * Called when the reactor detects an error condition on the file 
         * descriptor.
         */
        virtual void handleError(Reactor &reactor) = 0;
        
        /**
         * Called when the rector detects tht the file descriptor has been 
         * disconnected.
         */
        virtual void handleDisconnect(Reactor &reactor) = 0;
        
        /**
         * Returns the file descriptor associated with the reactable.
         */
        virtual int fileno() = 0;
};

SPUG_LPTR(Reactable);

} // namespace spug

#endif
