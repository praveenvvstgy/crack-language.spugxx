
#file SockStream

#intinclude Socket
#intinclude iostream

#impinclude Exception

#ifndef __IBMCPP__
#include <streambuf.h>
#endif

//	Anatomy of a Stream Buffer
//
//	The buffer part of a stream buffer is one large area which
// is divided up into get and put areas by the specialization class
//	derived from streambuf:
//						 _
//		pbase() ->	| |
//						| |
//						| |
//						| |	the stream buffer
//						| |
//						|_|
//		ebuf() ->			first byte following the buffer
//
//	Within the stream buffer, the derived class may define a get buffer and/or
//	a set buffer.  The get buffer is structured as follows:
//						 _
//		eback() ->	| |
//						| |	putback area.  Caller may "unget" to this point.
//						|_|
//		gptr() ->	| |
//						| |
//						| |	get area (stuff obtained from producer, but not yet
//						| |	consumed)
//						|_|
//		egptr() ->			first byte following get area
//
//	The put buffer is structured as follows:
//						 _
//		pbase() ->	| |
//						| |	characters put so far, but not yet given to ultimate
//						|_|	consumer
//		pptr() ->	| |
//						| |
//						| |	put area
//						| |
//						|_|
//		epptr() ->			first byte following put area.
//
//	To manipulate these buffers:
//
//		setb(base, ebuf, ownsIt)	sets the base and ebuf values, if "ownsIt"
//											delete it on the next setb().
//		setg(eback, gptr, ebuf) 	sets the get area, allowing for a putback area
//		setp(pbase, epptr) 	 		sets the put area.

class SockStreamBuf : public streambuf {

   public:
		/*---------------------------------------------------------------------*\
			creates a socket stream buffer from a socket.  Flags is used to
			indicate whether the stream will be for reading (ios::in), writing
			(ios::out), or both (ios::in | ios::out).
			'sock0' is internally referenced by the stream buffer - therefore,
			it is expected to remain in scope for as long as the SockStreamBuf
			remains in scope.
		\*---------------------------------------------------------------------*/
      SockStreamBuf(Socket &sock0,  int flags) :
				sock(sock0)
			{
			init(flags);
			}
			

   protected:
      /*---------------------------------------------------------------------*\
			dumps the currrent buffer to the socket, stores the overflow
			character in the buffer.
		\*---------------------------------------------------------------------*/
      virtual int overflow(int c = EOF)
			{
		   sync();
   
		   if (c != EOF) {
      		// store c in the put buffer
		      *pptr() = c;
      
		      // increment the put buffer pointer
		      pbump(1);
				}
		   return !EOF;
			}

			
      
      // --- Not defined for socket stream buffers ---
      // virtual streampos seekoff(streamoff so, seekdir dir, 
      //                           int mode = ios::in | ios::out);
      // virtual streampos seekpos(streampos pos,
      //                           int mode = ios::in | ios::out);
      
		/*---------------------------------------------------------------------*\
			"synchronizes" (flushes) the buffer with the ultimate producer.
			Writes all of the data in the buffer to the associated socket.
			
			Note: the IBM documentation of "sync()" suggests that this function
			should return 0 as an error condition (when there are no characters
			to write or no stream buffer) and EOF as the norm.  The GNU
			documentation suggests the opposite.   This is written to conform
			to GNU, and seems to work with it.   I do not know if it will
			work with IBMs io libarary.
		\*---------------------------------------------------------------------*/
      virtual int sync()
			{
		   // figure out how many characters are available for putting
		   int numChars = pptr() - pbase();
   
		   // if there are any, "send" them to the socket.
		   if (numChars) {
				sock.send(pbase(), numChars);
   
		      // reset the put area
		      setp(pbase(), epptr());
       
		      // indicate that we did something
		      return 0;
		      }
		   else
		      return EOF;
			}			

		/*---------------------------------------------------------------------*\
			Called when the stream buffer runs out of data.  This attempts to
			get a new buffer full of gettable data from the associated socket.
		\*---------------------------------------------------------------------*/
      virtual int underflow()
			{
			int retVal;
			if (gptr() == egptr())
				{
				// fill a buffer of the maximum receive area size from the
				// socket.
				int size = sock.recv(eback(), ebuf() - eback());
				if (!size)
					return EOF;
					
				// reset the get area size to the size of the block received.
				setg(eback(), eback(), eback() + size);
				}
			
			// get and return the next character.
			retVal = *gptr();
			return retVal;
			}
      
   private:
		/*---------------------------------------------------------------------*\
			Initializes the stream buffer using the usual incantations.  Defines
			the stream buffer as input, output, or both depending on whether
			flags includes ios::in and ios::out.
		\*---------------------------------------------------------------------*/
		void init(int flags)
			{
		   doallocate();
			if ((flags & (ios::in | ios::out)) == (ios::in | ios::out))
				{
				// if this is an input/output buffer, divide the buffer in half.
				// the second half must be for the get buffer.
				char *startOfGetArea = (ebuf() - base()) / 2 + base();
				setp(base(), startOfGetArea);
				setg(startOfGetArea, ebuf(), ebuf());
				}
			else if (flags & ios::in)
			   setg(base(), ebuf(), ebuf());
			else if (flags & ios::out)
				setp(base(), ebuf());
			else
				throw Exception("Attempted to initialize an SockStreamBuf as "
									 "neither read nor write.");
			}
			
      Socket &sock;
   }

class SockStream : public iostream {
	public:
		SockStream(Socket &sock, int flags = ios::in | ios::out) :
				iostream(new SockStreamBuf(sock, flags))
			{
			}
	}

class OSockStream : public ostream {
	public:
		OSockStream(Socket &sock) :
				ostream(new SockStreamBuf(sock, ios::out))
			{
			}
	}

class ISockStream : public istream {
	public:
		ISockStream(Socket &sock) :
				istream(new SockStreamBuf(sock, ios::in))
			{
			}
	}

