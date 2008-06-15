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
