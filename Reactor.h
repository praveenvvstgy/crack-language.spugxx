/*===========================================================================*\
    
    Reactor: interface for implementations of the reactor pattern.
    
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

#ifndef SPUG_REACTOR_H
#define SPUG_REACTOR_H

#include "LPtr.h"
#include "RCBase.h"

namespace spug {

class Reactable;
class Runnable;
class TimeDelta;

/**
 * Basic reactor interface.
 */
class Reactor : public virtual RCBase {

    public:
        
        /** 
         * Add a new reactable.
         * 
         * @param reactable 
         * Throws:
         *   spug::DuplicateException if the reactable is already being 
         *   managed.
         */
        virtual void addReactable(Reactable *reactable) = 0;

        /**
         * Remove an existing reactable from the set managed by the reactor.        
         * 
         * Throws:
         *   spug::DoesNotExist if the reactable is not being managed.
         */
        virtual void removeReactable(Reactable *reactable) = 0;
        
        /**
         * Returns true if the instance is managing any reactables.
         */
        virtual bool hasReactables() = 0;
        
        /**
         * Schedule an event to be run delta seconds from now.
         */
        virtual void schedule(const TimeDelta &delta, Runnable *runnable) = 0;
        
        /**
         * Wait for the next event and process it.
         * 
         * Returns true if an event was processed, immediately returns false 
         * if there are no reactables or scheduled events.
         */
        virtual bool processOneEvent() = 0;
        
        /**
         * Process events in a loop until there are no more reactables and no 
         * more scheduled events.
         */
        void run();
        
        /**
         * Create an instance of the default implementation of Reactor.
         */
        static Reactor *createReactor(); 

};

SPUG_LPTR(Reactor);

} // namespace spug

#endif
