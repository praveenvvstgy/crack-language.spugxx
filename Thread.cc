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

#include "Thread.h"

#include <pthread.h>
#include "Exception.h"
#include "InvalidStateException.h"
#include "Runnable.h"

using namespace spug;

void *Thread::outerFunction(void *arg) {
    ThreadLPtr thread = reinterpret_cast<Thread *>(arg);
    try {
        thread->target->run();
    } catch (const spug::Exception &ex) {
        std::cerr << "Exception at top of the thread:\n" << ex << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "std::exception at top of the thread:\n" << ex.what() 
                  << std::endl;
    } catch (...) {
        std::cerr << "unknown exception at the top of the thread."
                  << std::endl;
    }    
}

struct Thread::Internal {
    pthread_t thread;
};

Thread::Thread(Runnable *target) : 
    target(target), 
    state(preRun),
    internal(new Thread::Internal) {
}

Thread::~Thread() {
    if (state == running)
        join();
}

void Thread::start() {
    if (state != preRun)
        throw InvalidStateException("thread is running or terminated.");
    
    pthread_create(&internal->thread, NULL, &Thread::outerFunction, this);
    state = running;
}

void Thread::join() {
    if (state != running)
        throw InvalidStateException("thread is not running");
    pthread_join(internal->thread, NULL);
    state = terminated;
}
