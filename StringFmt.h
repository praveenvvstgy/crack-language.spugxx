/*===========================================================================*\

   $Id$

   This header mainly defines the macro SPUG_FMT() - which can be used to
   format a spug::String in-place using the stream operator:

      void foo(const spug::String &str);
      foo(SPUG_FSTR("this is an integer: " << 100 << " this is a float: " <<
                    1.0
                    )
          )

   Copyright (C) 2007 Michael A. Muller

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

#ifndef SPUG_STRINGFMT_H
#define SPUG_STRINGFMT_H

#include <sstream>
#include "String.h"

namespace spug {
   /**
    * For some reason, on gcc 4.1, an std::ostringstream temporary doesn't
    * behave the same as an ostring with regards to the "<<" operator - in
    * particular, 'std::ostringstream() << "string val"' will treat the string
    * as a void*, printing its address.
    * This class solves that problem.
    */
   class StringFmt {
      private:
         std::ostringstream out;

      public:
         template <typename T>
         std::ostream &operator <<(const T &val) {
            out << val;
            return out;
         }
   };
}

#define SPUG_FSTR(msg) \
      static_cast<const std::ostringstream&>(spug::StringFmt() << msg).str()

#endif
