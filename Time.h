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

#ifndef SPUG_TIME_H
#define SPUG_TIME_H

#include "TimeBase.h"

namespace spug {

class TimeDelta;

/**
 * Instances of this class define a point in time.
 */
struct Time : public TimeBase {
    Time() {}
    Time(int sec) : TimeBase(sec) {}
    Time(int sec, unsigned usec) : TimeBase(sec, usec) {}
    
    Time operator +(const TimeDelta &other) const;
    Time operator -(const TimeDelta &other) const;
    TimeDelta operator -(const Time &other) const;
    
    static Time now();
};

} // namespace spug

#endif
