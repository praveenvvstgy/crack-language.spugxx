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

#ifndef SPUG_INETADDRESS_H
#define SPUG_INETADDRESS_H

#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>

struct sockaddr;

#include "SockException.h"
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

namespace spug {

class InetAddress {
    private:
        unsigned long address;
    
    public:
    
        /**
         * Default constructor for InetAddress.   Sets the address to 0.
         */
        InetAddress() : address(0) {}
 
        /**
         * Constructs the address from a string, which may be either a
         * host name or a dot-seperated host address.
         */
        InetAddress(const char *nameOrNumber);
        
        /**
         * Constructs the address from a sockaddr structure.  The structure
         * must be of family AF_INET, and should really be a sockaddr_in
         * structure.
         */
        InetAddress(const sockaddr &addr, unsigned size);
        
        /**
         * Converts the address to a sockaddr_in structure, suitable for
         * use in low-level socket calls.   Port number is set to 0.
         */
        operator sockaddr_in() const;
        
};

} // namespace spug

#endif
