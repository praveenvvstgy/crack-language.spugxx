
#file Socket

#intinclude InetAddress

#impinclude SockException
#impinclude sys/socket
#impinclude errno
#impinclude memory

#>#ifdef __IBMCPP__
#>#include <utils.h>
#>#include <sys/socket.h>
#>#else
#>#include <unistd.h>
#>#endif

class Socket {
   
   public:
   
      enum Protocol { tcp, udp };
   
   private:
   
      enum { defaultMaxConnects = 5 };
   
      int sockHandle;
      bool hasPeer;
      InetAddress peerAddr;
      Protocol prot;

      /*---------------------------------------------------------------------*\
         Does the low level "socket()" call, opening the socket.
      \*---------------------------------------------------------------------*/
      void openSocket()
         {
         int baseProt;
         if (prot == tcp)
            baseProt = SOCK_STREAM;
         else
            baseProt = SOCK_DGRAM;
         sockHandle = socket(AF_INET, baseProt, 0);
         if (sockHandle == -1)
            throw SockException(SockException::open, errno);
         }

      /*---------------------------------------------------------------------*\
         Initializes the socket as a client connection.
      \*---------------------------------------------------------------------*/
      void initClient(int port)
         {
         openSocket();
         
         sockaddr_in addr = peerAddr;
         addr.sin_port = htons(port);
         if (connect(sockHandle, (sockaddr*)&addr, sizeof(addr)))
            throw SockException(SockException::connect, errno);
         }

   public:
         
      /*---------------------------------------------------------------------*\
         Constructs a client socket given an ascii inet address, a port, and
         optionally a protocol.  Connects to the given port.
      \*---------------------------------------------------------------------*/
      Socket(const char *addrName, int port, Socket::Protocol prot0 = tcp) :
            hasPeer(true),
            peerAddr(addrName),
            prot(prot0)
         {
         initClient(port);
         }

      /*---------------------------------------------------------------------*\
         Constructs a client socket given an ascii inet address, a port, and
         optionally a protocol.  Connects to the server at the given port.
      \*---------------------------------------------------------------------*/
      Socket(const InetAddress &addr, int port, Socket::Protocol prot0 = tcp) :
            hasPeer(true),
            peerAddr(addr),
            prot(prot0)
         {
         initClient(port);
         }
      
      /*---------------------------------------------------------------------*\
         Constructs a socket bound to a port.
      \*---------------------------------------------------------------------*/
      Socket(int portNum, Socket::Protocol prot0 = tcp) :
            hasPeer(false),
            prot(prot0)
         {
         openSocket();
         bind(portNum);
         }
      
      /*---------------------------------------------------------------------*\
         Constructs a socket from a socket handle and a peer address (if
         one exists).  This should generally only be used internally or by
         derived classes.
      \*---------------------------------------------------------------------*/
      Socket(int sockHandle0, bool hasPeer0, const InetAddress peerAddr0,
             Socket::Protocol prot0) :
            sockHandle(sockHandle0),
            hasPeer(hasPeer0),
            peerAddr(peerAddr0),
            prot(prot0)
         {
         }

      /*---------------------------------------------------------------------*\
         Destroys the socket.
      \*---------------------------------------------------------------------*/
      ~Socket()
         {
#ifdef __IBMCPP__
         soclose(sockHandle);
#else
         close(sockHandle);
#endif
         }

      /*---------------------------------------------------------------------*\
         Accepts a connection  on a socket which is bound and listening
         on a port.
      \*---------------------------------------------------------------------*/
      Socket accept() const
         {
         sockaddr_in addr;
         size_t size = sizeof(addr);
         int sock = ::accept(sockHandle, (sockaddr*)&addr, &size);
         if (sock != -1)
            return  Socket(sock,
                           true,
                           InetAddress((sockaddr&)addr, size),
                           tcp);
         else
            throw SockException(SockException::accept, errno);
         }

      /*---------------------------------------------------------------------*\
         Accepts a connection  on a socket which is bound and listening
         on a port.  Returns an orphaned allocated Socket object.  Management
         of the new Socket is the responsibility of the caller.
      \*---------------------------------------------------------------------*/
      Socket *acceptAlloc() const
         {
         sockaddr_in addr;
         size_t size = sizeof(addr);
         int sock = ::accept(sockHandle, (sockaddr*)&addr, &size);
         if (sock != -1)
            return  new Socket(sock,
                               true,
                               InetAddress((sockaddr&)addr, size),
                               tcp);
         else
            throw SockException(SockException::accept, errno);
         }

      /*---------------------------------------------------------------------*\
         Low-level socket "recv()" function.  Generally should not be used
         when it is possible to use the stream interface.
         Returns the length of the message if successful, or 0 if the
         connection has been closed.
      \*---------------------------------------------------------------------*/
      int recv(void *buffer, unsigned bufSize,  bool peek = false) const
         {
         int rc = ::recv(sockHandle, (char*)buffer, bufSize, peek ? MSG_PEEK : 0);
         if (rc == -1)
            throw SockException(SockException::recv, errno);
         return rc;
         }
      
      /*---------------------------------------------------------------------*\
         Low-level socket "send()" function.  Generally should not be used
         when it is possible to use the stream interface.  (return values
         are undefined)
      \*---------------------------------------------------------------------*/
      int send(void *buffer, unsigned bufSize) const
         {
         int rc = ::send(sockHandle, (char*)buffer, bufSize, 0);
         if (rc == -1)
            throw SockException(SockException::send, errno);
         return rc;
         }

      /*---------------------------------------------------------------------*\
         Low-level bind call - binds the socket to the port.  This
         should generally not need to be called, as it is done automatically
         by the constructor Socket(int portNum, Socket::Protocol prot0 = tcp)
      \*---------------------------------------------------------------------*/
      void bind(int portNum) const
         {
         sockaddr_in addr;
         memset(&addr, 0, sizeof(addr));
         addr.sin_family = AF_INET;
         addr.sin_port = htons(portNum);
         addr.sin_addr.s_addr = INADDR_ANY;
         if (::bind(sockHandle, (sockaddr*)&addr, sizeof(addr)))
            throw SockException(SockException::bind, errno);
         }
      
      /*---------------------------------------------------------------------*\
         Defines the socket as a "listen" socket - one which can accept
         connections.  This is *not* done automatically by the
         Socket(int portNum, Socket::Protocol prot0 = tcp) constructor,
         and must be done by any socket wishing to accept new TCP client
         connections.
            This function *is* called automatically by the ServerSocket()
         constructor.
            Accepts the maximum number of connections to queue as an optional
         parameter.
      \*---------------------------------------------------------------------*/
      void listen(int maxConnects = defaultMaxConnects) const
         {
         if (::listen(sockHandle, maxConnects))
            throw SockException(SockException::listen,  errno);
         }
      
      /*---------------------------------------------------------------------*\
         Provides external access to the low-level socket handle.
      \*---------------------------------------------------------------------*/
      int handle() const
         {
         return sockHandle;
         }
   }