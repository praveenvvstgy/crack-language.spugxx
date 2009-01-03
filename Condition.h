/*===========================================================================*\
     
    Copyright (C) 2009 Michael A. Muller
 
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

#ifndef SPUG_CONDITION_H
#define SPUG_CONDITION_H

#include <pthread.h>
#include "Mutex.h"

namespace spug {

class TimeDelta;
class Time;

/**
 * A condition variable: allows for signaling between threads with integration 
 * with a mutex.
 */
class Condition {
    private:
        Mutex &mutex;
        bool ownsMutex;
        pthread_cond_t condition;

    public:
        
        /**
         * Create a condition variable with its own mutex.
         */
        Condition() : mutex(*new Mutex()), ownsMutex(true) {
            pthread_cond_init(&condition, 0);
        }
        
        Condition(Mutex &mutex) : mutex(mutex), ownsMutex(false) {
            pthread_cond_init(&condition, 0);
        }
                  
        ~Condition() {
            pthread_cond_destroy(&condition);
            if (ownsMutex)
                delete &mutex;
        }
        
        /** Acquire the underlying mutex. */
        void acquire() {
            mutex.acquire();
        }
        
        /** Release the underlying mutex. */
        void release() {
            mutex.release();
        }
        
        /**
         * Notify at least one of the threads waiting on this condition to 
         * becom unblocked.
         */
        void notify();
        
        /**
         * Notify all of the threads waiting on this condition to become 
         * unblocked.
         */
        void notifyAll();
        
        /** 
         * Wait for the condition variable to be notified.  You must have 
         * acquired the mutex prior to calling this.
         */
        void wait();
        
        /**
         * Wait for the condition variable to be notified or until the 
         * expiration of a timeout.  You must have acquired the mutex prior to 
         * calling this.
         * 
         * Returns true if we got notification, false if not.
         */
        bool wait(const TimeDelta &timeout);
        
        /**
         * Wait for the condition variable to be notified or until the timeout 
         * time is reached.  You must have acquired the mutex priro to calling 
         * this.
         * 
         * Returns true if we got notification, false if we timed out.
         */
        bool wait(const Time &timeout);
};

} // namespace spug

#endif

