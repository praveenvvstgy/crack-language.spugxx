
#file ServerSocket

#intinclude Socket

class ServerSocket : public Socket {

	public:
		
		ServerSocket(int port, Socket::Protocol prot = tcp) :
				Socket(port, prot)
			{
			listen();
			}

	}