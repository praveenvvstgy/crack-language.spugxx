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

#include "Reactor.h"

#include <map>
#include <deque>
#include <poll.h>
#include <errno.h>
#include "DoesNotExistException.h"
#include "DuplicateException.h"
#include "Reactable.h"
#include "Runnable.h"
#include "StringFmt.h"
#include "SystemException.h"
#include "Time.h"
#include "TimeDelta.h"

using namespace spug;

namespace {

class PollReactor : public Reactor {
    private:
      
        struct Event {
            Time time;
            RunnableLPtr runnable;
            Event(Time time, Runnable *runnable) : 
                time(time), 
                runnable(runnable) {
            }
        };
      
        // the set of reactables that we manage
        typedef std::map<int, ReactableLPtr> ReactableMap;
        ReactableMap managed;
        
        // queue of events
        typedef std::deque<Event> EventQueue;
        EventQueue events;
        
        pollfd *fds;

    public:
  
        PollReactor() : fds(0) {}
      
        virtual void addReactable(Reactable *reactable) {
            int fd = reactable->fileno();
            if (managed.find(fd) != managed.end())
                throw DuplicateException(SPUG_FSTR("Reactable for fd " << 
                                                   fd <<
                                                   " already added."
                                                   )
                                         );
            
            managed[fd] = reactable;
            if (fds) {
                delete [] fds;
                fds = 0;
            }
        }
        
        virtual void removeReactable(Reactable *reactable) {
            int fd = reactable->fileno();
            if (managed.find(fd) == managed.end())
                throw DoesNotExistException(SPUG_FSTR("Reactable for fd " << 
                                                      fd <<
                                                      " is not being managed."
                                                      )
                                            );
            
            managed.erase(fd);
            if (fds) {
                delete [] fds;
                fds = 0;
            }
        }
        
        virtual bool hasReactables() {
            return managed.size() ? true : false;
        }
        
        virtual void schedule(const TimeDelta &time, Runnable *event) {
            Time t = Time::now() + time;

            // find where the new event belongs in the queue.
            EventQueue::iterator iter;
            for (iter = events.begin();
                 iter != events.end() && iter->time < t;
                 ++iter
                 );
            
            // and stick it there.
            events.insert(iter, Event(t, event));
        }
        
        virtual bool processOneEvent() {
            
            int fdCount = managed.size();
            
            // make sure that there's something for us to do...
            if (!fdCount && events.empty())
                return false;
            
            // reallocate the fds array if necessary
            if (!fds)
                fds = new pollfd[fdCount];
            
            // refill the poll structures
            int i = 0;
            for (ReactableMap::iterator iter = managed.begin();
                 iter != managed.end();
                 ++iter, ++i
                 ) {
                fds[i].fd = iter->second->fileno();
                
                // figure out what the events are.
                Reactable::Status status = iter->second->getStatus();
                short events = 0;
                if (status) 
                    events = POLLHUP | POLLERR | POLLNVAL;
                if (status & Reactable::readyToRead)
                    events |= POLLIN;
                if (status & Reactable::readyToWrite)
                    events |= POLLOUT;
                fds[i].events = events;
            }
            
            // if there are events, calculate the timeout until the next event.
            int timeout = -1;
            if (events.size()) {
                timeout = (events.front().time - Time::now()).toMillis();
                
                // if we've already timed out, we're done.
                if (timeout < 0) {
                    events.front().runnable->run();
                    events.pop_front();
                    return true;
                }
            }
            
            // do the poll
            int rc = poll(fds, fdCount, timeout);

            if (rc > 0) {
                // dispatch result
                for (int i = 0; i < fdCount; ++i) {
                    short revents = fds[i].revents;
                    if (revents & POLLOUT) {
                        managed[fds[i].fd]->handleWrite(*this);
                    } else if (revents & POLLIN) {
                        managed[fds[i].fd]->handleRead(*this);
                    } else if (revents & POLLHUP) {
                        managed[fds[i].fd]->handleDisconnect(*this);
                    } else if (revents & POLLNVAL ||
                            revents & POLLERR
                            ) {
                        managed[fds[i].fd]->handleError(*this);
                    }
                }
                
                return true;
            } else if (rc < 0) {
                throw SystemException(errno, "poll");
            } else {
                // timed out - run the next event.
                events.front().runnable->run();
                events.pop_front();
                return true;
            }
        }
};

} // anon namespace


void Reactor::run() {
    while (processOneEvent()) ;
}

Reactor *Reactor::createReactor() {
    return new PollReactor();
}
