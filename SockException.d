
#file SockException

#intinclude Exception

#impinclude errno

class SockException : public Exception {
	
	public:
		enum Action { open, connect, bind, send, recv, close, accept, listen,
						  gethostbyname,  select };
		enum SpecialError { unknownAddressFamily = 1000,  undefinedHost };
	
	private:
		Action _action;
		int _errno;
		
	public:
	
		/*---------------------------------------------------------------------*\
			Constructs a SockException.
		\*---------------------------------------------------------------------*/
		SockException(SockException::Action action0, int errno0) :
				_action(action0),
				_errno(errno0)
			{
			}

		/*---------------------------------------------------------------------*\
			Copy constructor for SockException.
		\*---------------------------------------------------------------------*/
		SockException(const SockException &other) :
				_action(other._action),
				_errno(other._errno)
			{
			}
		
		/*---------------------------------------------------------------------*\
			Creates a string from the action and error codes.
		\*---------------------------------------------------------------------*/
		virtual String asString() const
			{
			const char *actionStr, *errorStr;
			switch (_action)
				{
				case open:
					actionStr = "open";
					break;
					
				case connect:
					actionStr = "connect";
					break;
					
				case bind:
					actionStr = "bind";
					break;
				
				case send:
					actionStr = "send";
					break;
				
				case recv:
					actionStr = "recv";
					break;
				
				case close:
					actionStr = "close";
					break;
				
				case accept:
					actionStr = "accept";
					break;
				
				case listen:
					actionStr = "listen";
					break;
				
				case gethostbyname:
					actionStr = "gethostbyname";
					break;
				
				case select:
					actionStr = "select";
					break;
				
				default:
					actionStr = "forgot to specify an action!!!";
					break;
				}


			// This part really belongs in a generic system error handling class.
			switch (_errno)
				{
#ifndef __IBMCPP__
				case EPERM:
					errorStr = "Operation not permitted";
					break;
				
				case ENOENT:
					errorStr = "No such file or directory";
					break;
				
				case ESRCH:
					errorStr = "No such process";
					break;
				
				case EINTR:
					errorStr = "Interrupted system call";
					break;
				
				case EIO:
					errorStr = "I/O error";
					break;
				
				case ENXIO:
					errorStr = "No such device or address";
					break;
				
				case E2BIG:
					errorStr = "Arguments or environment too big";
					break;
				
				case ENOEXEC:
					errorStr = "Invalid executable file format";
					break;
				
				case EBADF:
					errorStr = "Bad file number";
					break;
				
				case ECHILD:
					errorStr = "No children";
					break;
				
				case EAGAIN:
					errorStr = "Resource temporarily unavailable";
					break;
				
				case ENOMEM:
					errorStr = "Not enough memory";
					break;
				
				case EACCES:
					errorStr = "Permission denied";
					break;
				
				case EFAULT:
					errorStr = "Bad address";
					break;
				
				case ENOLCK:
					errorStr = "No locks available";
					break;
				
				case EBUSY:
					errorStr = "Resource busy";
					break;
				
				case EEXIST:
					errorStr = "File exists";
					break;
				
				case EXDEV:
					errorStr = "Cross-device link";
					break;
				
				case ENODEV:
					errorStr = "No such device";
					break;
				
				case ENOTDIR:
					errorStr = "Not a directory";
					break;
				
				case EISDIR:
					errorStr = "Is a directory";
					break;
				
				case EINVAL:
					errorStr = "Invalid argument";
					break;
				
				case ENFILE:
					errorStr = "Too many open files in system";
					break;
				
				case EMFILE:
					errorStr = "Too many open files";
					break;
				
				case ENOTTY:
					errorStr = "Inappropriate ioctl";
					break;
				
				case EDEADLK:
					errorStr = "Resource deadlock avoided";
					break;
				
				case EFBIG:
					errorStr = "File too large";
					break;
				
				case ENOSPC:
					errorStr = "Disk full";
					break;
				
				case ESPIPE:
					errorStr = "Illegal seek";
					break;
				
				case EROFS:
					errorStr = "Read-only file system";
					break;
				
				case EMLINK:
					errorStr = "Too many links";
					break;
				
				case EPIPE:
					errorStr = "Broken pipe";
					break;
				
				case EDOM:
					errorStr = "Domain error";
					break;
				
				case ERANGE:
					errorStr = "Result too large";
					break;
				
				case ENOTEMPTY:
					errorStr = "Directory not empty";
					break;
				
				case EINPROGRESS:
					errorStr = "Operation now in progress";
					break;
				
				case ENOSYS:
					errorStr = "Function not implemented";
					break;
				
				case ENAMETOOLONG:
					errorStr = "File name too long";
					break;
				
				case EDESTADDRREQ:
					errorStr = "Destination address required";
					break;
				
				case EMSGSIZE:
					errorStr = "Message too long";
					break;
				
				case EPROTOTYPE:
					errorStr = "Protocol wrong type for socket";
					break;
				
				case ENOPROTOOPT:
					errorStr = "Option not supported by protocol";
					break;
				
				case EPROTONOSUPPORT:
					errorStr = "Protocol not supported";
					break;
				
				case ESOCKTNOSUPPORT:
					errorStr = "Socket type not supported";
					break;
				
				case EOPNOTSUPP:
					errorStr = "Operation not supported on socket";
					break;
				
				case EPFNOSUPPORT:
					errorStr = "Protocol family not supported";
					break;
				
				case EAFNOSUPPORT:
					errorStr = "Address family not supported by protocol family";
					break;
				
				case EADDRINUSE:
					errorStr = "Address already in use";
					break;
				
				case EADDRNOTAVAIL:
					errorStr = "can't assigned requested address";
					break;
				
				case ENETDOWN:
					errorStr = "Network is down";
					break;
				
				case ENETUNREACH:
					errorStr = "Network is unreachable";
					break;
				
				case ENETRESET:
					errorStr = "Network dropped connection on reset";
					break;
				
				case ECONNABORTED:
					errorStr = "Software caused connection abort";
					break;
				
				case ECONNRESET:
					errorStr = "Connection reset by peer";
					break;
				
				case ENOBUFS:
					errorStr = "No buffer space available";
					break;
				
				case EISCONN:
					errorStr = "Socket is already connected";
					break;

				case ENOTCONN:
					errorStr = "Socket is not connected";
					break;
				
				case ESHUTDOWN:
					errorStr = "Can't send after socket shutdown";
					break;
				
				case ETOOMANYREFS:
					errorStr = "Too many references: can't splice";
					break;
				
				case ETIMEDOUT:
					errorStr = "Connection timed out";
					break;
				
				case ECONNREFUSED:
					errorStr = "Connection refused";
					break;
				
				case ELOOP:
					errorStr = "Too many levels of symbolic links";
					break;
				
				case ENOTSOCK:
					errorStr = "Socket operation on non-socket";
					break;
				
				case EHOSTDOWN:
					errorStr = "Host is down";
					break;
				
				case EHOSTUNREACH:
					errorStr = "No route to host";
					break;
				
				case EALREADY:
					errorStr = "Operation already in progress";
					break;

#endif

				// special errors

				case unknownAddressFamily:
					errorStr = "Unknown address family";
					break;
				
				case undefinedHost:
					errorStr = "Undefined host.";
					break;
				
				default:
					errorStr = "Unknown Error Code.";
					break;
				}

			return String("Error occured during ") + actionStr +
				": " + errorStr;
			}

	}


