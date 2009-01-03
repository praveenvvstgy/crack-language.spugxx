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

#include "Condition.h"

#include <errno.h>
#include "SystemException.h"
#include "Time.h"
#include "TimeDelta.h"

using namespace spug;

void Condition::notify() {
    int rc = pthread_cond_signal(&condition);
    if (rc)
        throw SystemException(rc, "Condition::notify");
}

void Condition::notifyAll() {
    int rc = pthread_cond_broadcast(&condition);
    if (rc)
        throw SystemException(rc, "Condition::notifyAll");
}

void Condition::wait() {
    int rc = pthread_cond_wait(&condition, &mutex.mutex);
    if (rc)
        throw SystemException(rc, "Condition::wait");
}

bool Condition::wait(const TimeDelta &timeout) {
    Time absTimeout = Time::now() + timeout;
    timespec ts;
    ts.tv_sec = absTimeout.getSeconds();
    ts.tv_nsec = absTimeout.getMicroseconds() * 1000;
    int rc = pthread_cond_timedwait(&condition, &mutex.mutex, &ts);
    if (rc == ETIMEDOUT)
        return false;
    else if (rc)
        throw SystemException(rc, "Condition::wait");
    else
        return true;
}

bool Condition::wait(const Time &timeout) {
    timespec ts;
    ts.tv_sec = timeout.getSeconds();
    ts.tv_nsec = timeout.getMicroseconds() * 1000;
    int rc = pthread_cond_timedwait(&condition, &mutex.mutex, &ts);
    if (rc == ETIMEDOUT)
        return false;
    else if (rc)
        throw SystemException(rc, "Condition::wait");
    else
        return true;
}
