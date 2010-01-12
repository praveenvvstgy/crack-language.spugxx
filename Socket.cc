
#include "Socket.h"

#include "InetAddress.h"
#include "SockException.h"
#include <sys/socket.h>
#include <errno.h>
#include <memory.h>

#ifdef __IBMCPP__
# include <utils.h>
# include <sys/socket.h>
#else
# include <unistd.h>
#endif

using namespace spug;

void Socket::openSocket() {
    int baseProt;
    if (prot == tcp)
        baseProt = SOCK_STREAM;
    else
        baseProt = SOCK_DGRAM;
    sockHandle = socket(AF_INET, baseProt, 0);
    if (sockHandle == -1)
        throw SockException(SockException::open, errno);
}

void Socket::initClient(int port) {
    openSocket();
    
    sockaddr_in addr = peerAddr;
    addr.sin_port = htons(port);
    if (connect(sockHandle, (sockaddr*)&addr, sizeof(addr)))
        throw SockException(SockException::connect, errno);
}

Socket Socket::accept() const {
    sockaddr_in addr;
    size_t size = sizeof(addr);
    int sock = ::accept(sockHandle, (sockaddr*)&addr, (socklen_t *)&size);
    if (sock != -1)
        return Socket(sock, true, InetAddress((sockaddr&)addr, size),
  	              tcp
                      );
    else
        throw SockException(SockException::accept, errno);
}

Socket *Socket::acceptAlloc() const {
    sockaddr_in addr;
    size_t size = sizeof(addr);
    int sock = ::accept(sockHandle, (sockaddr*)&addr, (socklen_t *)&size);
    if (sock != -1)
        return  new Socket(sock, true, InetAddress((sockaddr&)addr, size),
                           tcp
                           );
    else
        throw SockException(SockException::accept, errno);
}

int Socket::recv(void *buffer, unsigned bufSize,  bool peek) const {
    int rc = ::recv(sockHandle, (char*)buffer, bufSize, peek ? MSG_PEEK : 0);
    if (rc == -1)
        throw SockException(SockException::recv, errno);
    return rc;
}
      
int Socket::send(void *buffer, unsigned bufSize) const {
    int rc = ::send(sockHandle, (char*)buffer, bufSize, 0);
    if (rc == -1)
        throw SockException(SockException::send, errno);
    return rc;
}

void Socket::bind(int portNum) const {
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portNum);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(sockHandle, (sockaddr*)&addr, sizeof(addr)))
        throw SockException(SockException::bind, errno);
}
      
void Socket::listen(int maxConnects) const {
    if (::listen(sockHandle, maxConnects))
        throw SockException(SockException::listen,  errno);
}

int Socket::getPort() const {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getsockname(sockHandle, (struct sockaddr *)&addr, &len);
    return ntohs(addr.sin_port);
}
