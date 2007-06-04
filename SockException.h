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
