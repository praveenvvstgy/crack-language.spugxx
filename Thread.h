/*===========================================================================*\
     
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

#ifndef SPUG_THREAD_H
#define SPUG_THREAD_H

#include "LPtr.h"
#include "RCBase.h"

namespace spug {

SPUG_LPTR(Runnable);
SPUG_LPTR(Thread);

/**
 * A thread.  Threads run Runnables and own a reference to them.
 */
class Thread : public RCBase {
    private:
        RunnableLPtr target;
        typedef enum {preRun, running, terminated} State;
        State state;
        struct Internal;
        Internal *internal;

        static void *outerFunction(void *);

    public:
        /**
         * Constructs a new thread with its own runnable.
         */
        Thread(Runnable *target);
        
        ~Thread();
        
        /** 
         * Set the runnable that gets run by the thread.  This must be run 
         * before the thread is started - otherwise it throws 
         * InvalidStateException.
         */
        void setTarget(Runnable *target);
        
        /** Start running the thread. */
        void start();

        /** 
         * Join the thread: wait for it to terminate and then collect its 
         * datastructures.
         */
        void join();
};

} // namespace spug

#endif
