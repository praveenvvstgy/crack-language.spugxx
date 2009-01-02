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

#ifndef SPUG_TIMEDELTA_H
#define SPUG_TIMEDELTA_H

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "TimeBase.h"

namespace spug {

/**
 * TimeDelta is a difference between two points in time.  It consists of a 
 * seconds component and a microseconds component.
 */
struct TimeDelta : public TimeBase {

    public:
        /** Creates a time delta of zero. */
        TimeDelta() {}
        
        /** Creates a time delta of the specified number of seconds. */
        TimeDelta(int sec) : TimeBase(sec) {}
        
        /** 
         * Creates a time delta of the specified combination of seconds and 
         * milliseconds.  Normalizes the values so that the two components are 
         * always of the same sign and so that microseconds is never greater 
         * than one million or less than negative one million.
         */
        TimeDelta(int sec, int usec) : TimeBase(sec, usec) {}
    
        TimeDelta operator -(const TimeDelta &other) const {
            TimeDelta result = *this;
            result.subtract(other);
            return result;
        }
        
        TimeDelta operator +(const TimeDelta &other) const {
            TimeDelta result = *this;
            result.add(other);
            return result;
        }
        
        Time operator +(const Time &other) const {
            return other + *this;
        }
};

} // namespace spug

#endif
