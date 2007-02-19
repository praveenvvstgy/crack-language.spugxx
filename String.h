/*===========================================================================*\

    A very limited extension of std::string.

    Copyright (C) 2006 Michael A. Muller

    Permission is granted to use, modify and redistribute this code,
    providing that the following conditions are met:
 
    1) This copyright/licensing notice must remain intact.
    2) If the code is modified and redistributed, the modifications must 
    include documentation indicating that the code has been modified.
    3) The author(s) of this code must be indemnified and held harmless
    against any damage resulting from the use of this code.

    This code comes with ABSOLUTELY NO WARRANTEE, not even the implied 
    warrantee of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

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

        operator const char *() { return c_str(); }

        String operator +(const std::string &other) const {
            return (const String&)std::operator +(*this, other);
        }

        String operator +(const char *other) const {
            return (const String&)std::operator +(*this, other);
        }
};

} // namespace spug


#endif
