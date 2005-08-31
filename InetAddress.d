
#file InetAddress

#intinclude ctype
#intinclude sys/types
#intinclude netinet/in

struct sockaddr;

#impinclude SockException
#impinclude netdb
#impinclude string
#impinclude sys/socket

#>#ifdef __IBMCPP__
#>#else
#>#include <arpa/inet.h>
#>#endif

class InetAddress {
	private:
		unsigned long address;
	
	public:
	
		/*---------------------------------------------------------------------*\
			Default constructor for InetAddress.   Sets the address to 0.
		\*---------------------------------------------------------------------*/
		InetAddress() : address(0)
			{
			}

		/*---------------------------------------------------------------------*\
			Constructs the address from a string, which may be either a
			host name or a dot-seperated host address.
		\*---------------------------------------------------------------------*/
		InetAddress(const char *nameOrNumber)
			{
			if (isdigit(*nameOrNumber))
				address = inet_addr((char*)nameOrNumber);
			else
				{
				hostent *entry = gethostbyname((char*)nameOrNumber);
				if (!entry)
					throw SockException(SockException::gethostbyname,
											  SockException::undefinedHost);
				address = *(unsigned long*)entry->h_addr;
				}
			}
		
		/*---------------------------------------------------------------------*\
			Constructs the address from a sockaddr structure.  The structure
			must be of family AF_INET, and should really be a sockaddr_in
			structure.
		\*---------------------------------------------------------------------*/
		InetAddress(const sockaddr &addr, unsigned size)
			{
			if (size < sizeof(sockaddr_in) ||
				 ((const sockaddr_in&)addr).sin_family != AF_INET)
				throw SockException(SockException::accept,
										  SockException::unknownAddressFamily);
			address = ((const sockaddr_in&)addr).sin_addr.s_addr;
			}
		
		/*---------------------------------------------------------------------*\
			Converts the address to a sockaddr_in structure, suitable for
			use in low-level socket calls.   Port number is set to 0.
		\*---------------------------------------------------------------------*/
		operator sockaddr_in() const
			{
			sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = 0;
			addr.sin_addr.s_addr = address;
			return addr;
			}
		
	}