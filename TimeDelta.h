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

namespace spug {

/**
 * TimeDelta is a difference between two points in time.  It consists of a 
 * seconds component and a microseconds component.
 */
struct TimeDelta {

    private:
        // seconds
        int sec;
        
        // microseconds
        int usec;

        void normalize() {
            
            // convert u-sec overflow into seconds
            if (usec >= 1000000) {
                usec -= 1000000;
                ++sec;
            } else if (usec <= -1000000) {
                usec += 1000000;
                --sec;
            }
            
            // make sure the signs are compatible
            if (usec < 0 && sec > 0) {
                usec = 1000000 + usec;
                --sec;
            } else if (usec > 0 && sec < 0) {
                usec = -1000000 + usec;
                ++sec;
            }
        }

    public:
        friend 
            std::ostream &operator <<(std::ostream &out, const TimeDelta &td);

        /** Creates a time delta of zero. */
        TimeDelta() : sec(0), usec(0) {}
        
        /** Creates a time delta of the specified number of seconds. */
        TimeDelta(int sec) : sec(sec), usec(0) {}
        
        /** 
         * Creates a time delta of the specified combination of seconds and 
         * milliseconds.  Normalizes the values so that the two components are 
         * always of the same sign and so that microseconds is never greater 
         * than one million or less than negative one million.
         */
        TimeDelta(int sec, int usec) : sec(sec), usec(usec) {
            normalize();
        }
    
        TimeDelta operator -(const TimeDelta &other) const {
            TimeDelta result;
            result.sec = sec - other.sec;
            result.usec = usec - other.usec;
            result.normalize();
            return result;
        }
        
        TimeDelta operator +(const TimeDelta &other) const {
            TimeDelta result;
            result.sec = sec + other.sec;
            result.usec = usec + other.usec;
            result.normalize();
            return result;
        }
        
        bool operator ==(const TimeDelta &other) {
            return sec == other.sec && usec == other.usec;
        }
        
        bool operator !=(const TimeDelta &other) {
            return sec != other.sec || usec != other.usec;
        }
    
        /** Return the number of millisec in the time delta. */
        int toMillis() const {
            return sec * 1000 + usec / 1000;
        }
        
        /** Returns the seconds component of the delta. */
        int getSeconds() const {
            return sec;
        }
        
        /** Returns the microseconds component of the delta. */
        int getMicroseconds() const {
            return usec;
        }
        
};

inline std::ostream &operator <<(std::ostream &out, const TimeDelta &td) {
    if (!td.sec && td.usec < 0) out << '-';
    out << td.sec << "." << std::setw(6) << std::setfill('0') << abs(td.usec);
    return out;
}

} // namespace spug

#endif
