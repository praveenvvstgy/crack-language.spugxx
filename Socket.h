/*===========================================================================*\

   Basic socket implementation.

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

#ifndef SPUG_SOCKET_H
#define SPUG_SOCKET_H

#include <InetAddress.h>

namespace spug {

class Socket {
    
    public:
    
        enum Protocol { tcp, udp };
    
    private:
    
        enum { defaultMaxConnects = 5 };
     
        int sockHandle;
        bool hasPeer;
        InetAddress peerAddr;
        Protocol prot;
  
	/**
         * Does the low level "socket()" call, opening the socket.
	 */
        void openSocket();
  
	/**
         * Initializes the socket as a client connection.
	 */
        void initClient(int port);
  
    public:
          
	/**
         * Constructs a client socket given an ascii inet address, a port, and
         * optionally a protocol.  Connects to the given port.
         */
        Socket(const char *addrName, int port, Socket::Protocol prot0 = tcp) :
	        hasPeer(true),
	        peerAddr(addrName),
	        prot(prot0) {
            initClient(port);
	}
  
	/**
         * Constructs a client socket given an ascii inet address, a port, and
         * optionally a protocol.  Connects to the server at the given port.
         */
        Socket(const InetAddress &addr, int port, 
	       Socket::Protocol prot0 = tcp
	       ) :
	        hasPeer(true),
	        peerAddr(addr),
	        prot(prot0) {
            initClient(port);
	}
        
	/**
         * Constructs a socket bound to a port.
         */
        Socket(int portNum, Socket::Protocol prot0 = tcp) :
                hasPeer(false),
                prot(prot0) {
            openSocket();
            bind(portNum);
	}
        
	/**
         * Constructs a socket from a socket handle and a peer address (if
         * one exists).  This should generally only be used internally or by
         * derived classes.
         */
        Socket(int sockHandle0, bool hasPeer0, const InetAddress peerAddr0,
                Socket::Protocol prot0) :
                sockHandle(sockHandle0),
                hasPeer(hasPeer0),
                peerAddr(peerAddr0),
                prot(prot0) {
	}
  
	/**
         * Destroys the socket.
         */
        ~Socket() {
 #ifdef __IBMCPP__
            soclose(sockHandle);
 #else
            close(sockHandle);
 #endif
	}
 
	/**
         * Accepts a connection  on a socket which is bound and listening
         * on a port.
         */
        Socket accept() const;
  
        /**
         * Accepts a connection  on a socket which is bound and listening
         * on a port.  Returns an orphaned allocated Socket object.  Management
         * of the new Socket is the responsibility of the caller.
         */
        Socket *acceptAlloc() const;
  
        /**
         * Low-level socket "recv()" function.  Generally should not be used
         * when it is possible to use the stream interface.
         * Returns the length of the message if successful, or 0 if the
         * connection has been closed.
         */
        int recv(void *buffer, unsigned bufSize,  bool peek = false) const;
        
        /**
         * Low-level socket "send()" function.  Generally should not be used
         * when it is possible to use the stream interface.  (return values
         * are undefined)
         */
        int send(void *buffer, unsigned bufSize) const;
  
        /**
         * Low-level bind call - binds the socket to the port.  This
         * should generally not need to be called, as it is done automatically
         * by the constructor Socket(int portNum, Socket::Protocol prot0 = tcp)
         */
        void bind(int portNum) const;
        
        /**
         * Defines the socket as a "listen" socket - one which can accept
         * connections.  This is *not* done automatically by the
         * Socket(int portNum, Socket::Protocol prot0 = tcp) constructor,
         * and must be done by any socket wishing to accept new TCP client
         * connections.
         *    This function *is* called automatically by the ServerSocket()
         * constructor.
         *    Accepts the maximum number of connections to queue as an optional
         * parameter.
         */
        void listen(int maxConnects = defaultMaxConnects) const;
        
        /**
         * Provides external access to the low-level socket handle.
         */
        int handle() const {
           return sockHandle;
        }

        /**
         * Returns the port that the socket is bound to locally.
         */
        int getPort() const;
};

} // namespace spug

#endif
