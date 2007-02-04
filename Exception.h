/*===========================================================================*\

   $Id$


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

#ifndef SPUG_EXCEPTION_H
#define SPUG_EXCEPTION_H

#include <exception>
#include <string>
#include <iostream>
#include "TypeInfo.h"

namespace spug {

/**
 * Simple exception base class that allows exceptions to be thrown with an
 * informative message.
 */
class Exception : public std::exception {

   private:
      std::string msg;

   public:
      Exception() {}
      Exception(const char *msg) : msg(msg) {}
      Exception(const std::string &msg) : msg(msg) {}

      ~Exception() throw () {}

      virtual const char *getClassName() const { 
         return TypeInfo::get(*this)->getName(); 
      }

      friend std::ostream &
	 operator <<(std::ostream &out, const Exception &err);
};

inline std::ostream &operator <<(std::ostream &out, const Exception &err) {
   out << err.getClassName() << ": " << err.msg;
   return out;
}

// some macros to make it extremely easy to define derived exceptions

// Defines an exception class derived from an arbitrary base class
#define SPUG_DERIVED_EXCEPTION(cls, base) \
   class cls : public base { \
      public: \
         cls() {} \
	 cls(const char *msg) : base(msg) {} \
	 cls(const std::string &msg) : base(msg) {} \
	 virtual const char *getClassName() const { return #cls; } \
   };

// defines an exception class derived from spug::Exception
#define SPUG_EXCEPTION(cls) SPUG_DERIVED_EXCEPTION(cls, spug::Exception)

}

#endif

