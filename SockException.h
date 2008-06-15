/*===========================================================================*\
     
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

#ifndef SPUG_SOCKEXCEPTION_H
#define SPUG_SOCKEXCEPTION_H

#include <Exception.h>
#include <String.h>

namespace spug {

/**
 * Raised when a socket error occurs.
 * @todo should be deriving from SystemException
 */
class SockException : public Exception {
   
   public:
       enum Action { open, connect, bind, send, recv, close, accept, listen,
                     gethostbyname,  select };
       enum SpecialError { unknownAddressFamily = 1000,  undefinedHost };
   
   private:
       Action _action;
       int _errno;

       static String asString(SockException::Action action, int errNo);
       
   public:
   
       /**
        * Constructs a SockException.
        */
       SockException(SockException::Action action0, int errno0) :
              _action(action0),
              _errno(errno0),
              Exception(asString(action0, errno0)) {
       }

       /**
        * Copy constructor for SockException.
        */
       SockException(const SockException &other) :
              _action(other._action),
              _errno(other._errno) {
       }
       
       /**
        * Creates a string from the action and error codes.
        */
       virtual String asString() const {
          return asString(_action, _errno);
       }
 
};

} // namespace spug

#endif
