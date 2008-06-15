/*===========================================================================*\
     
    Copyright (C) 2007 Michael A. Muller
 
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

#ifndef SPUG_SYSTEMEXCEPTION_H
#define SPUG_SYSTEMEXCEPTION_H

#include "Exception.h"

namespace spug {

/**
 * An exception thrown as the result of a failed system function.  These have
 * an error code that should be the value of "errno" after the failure.
 */
class SystemException : public Exception {
   private:
      int errorCode;

   public:
      SystemException(int errorCode, const char *msg) : 
         errorCode(errorCode),
         Exception(msg) {
      }

      virtual std::string getMessage() const;
};

} // namespace spug

#endif
