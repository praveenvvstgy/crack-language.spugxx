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

#include "InetAddress.h"

#include "SockException.h"
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#ifdef __IBMCPP__
#else
# include <arpa/inet.h>
#endif

using namespace spug;

InetAddress::InetAddress(const char *nameOrNumber) {
    if (isdigit(*nameOrNumber))
        address = inet_addr((char*)nameOrNumber);
    else {
        hostent *entry = gethostbyname((char*)nameOrNumber);
        if (!entry)
           throw SockException(SockException::gethostbyname,
                               SockException::undefinedHost);
        address = *(unsigned long*)entry->h_addr;
    }
}
      
InetAddress::InetAddress(const sockaddr &addr, unsigned size) {
    if (size < sizeof(sockaddr_in) ||
        ((const sockaddr_in&)addr).sin_family != AF_INET)
        throw SockException(SockException::accept,
                            SockException::unknownAddressFamily);
    address = ((const sockaddr_in&)addr).sin_addr.s_addr;
}
      
InetAddress::operator sockaddr_in() const {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = address;
    return addr;
}
