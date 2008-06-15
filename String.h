/*===========================================================================*\

    A very limited extension of std::string.

    Copyright (C) 2006 Michael A. Muller
 
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

#ifndef SPUG_STRING_H
#define SPUG_STRING_H

#if 0
/**
 * A lightweight string interface.  the String class is just a pointer to a
 * string object that provides an implementation optimized for a particular
 * purpose.  The intent is to provide a string that is the best of all worlds.
 */
class String {
   private:
      StringBase *rep;

   public:
      String() : rep(0) {}
      String(const String &other) : rep(other.rep ? other.rep->dup() : 0) {}
      ~String() {
	 if (rep)
	    rep->release();
      }
      
      String operator +(const String &other) {
	 if (!rep || !other.rep)
	    nullParam();
	 return rep->concat(other.rep);
      }

      size_t length() { return rep->length(); }
};
#endif

#include <string>

namespace spug {

class String : public std::string {
    public:
        String() {}
        String(const char *text) : std::string(text) {}
        String(const std::string &other) : std::string(other) {}

        operator const char *() { return c_str(); }

        String operator +(const std::string &other) const {
            String temp = *this;
            temp += other;
            return temp;
        }

        String operator +(const char *other) const {
            String temp = *this;
            temp += other;
            return temp;
        }
};

} // namespace spug


#endif
