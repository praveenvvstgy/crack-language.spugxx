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
