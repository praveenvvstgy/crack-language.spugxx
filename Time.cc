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

#include <sys/time.h>
#include <time.h>
#include "Time.h"
#include "TimeDelta.h"

using namespace spug;

Time Time::operator +(const TimeDelta &other) const {
    Time result = *this;
    result.add(other);
    return result;
}

Time Time::operator -(const TimeDelta &other) const {
    Time result = *this;
    result.subtract(other);
    return result;
}

TimeDelta Time::operator -(const Time &other) const {
    Time result(*this);
    result.subtract(other);
    return TimeDelta(result.sec, result.usec);
}

Time Time::now() {
    timeval tv;
    gettimeofday(&tv, 0);
    return Time(tv.tv_sec, tv.tv_usec);
}

