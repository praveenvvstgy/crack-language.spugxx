

#file SockSet

#intinclude SLList

class Socket;

#impinclude Socket
#>#if defined(__linux__)
#impinclude sys/types
#>#elif defined(__os2__)
#impinclude sys/select
#>#elif defined(__IBMCPP__)
#>#ifndef __SELECT_32H
#>#define BSD_SELECT
#>#include <sys/select.h>
#>#endif
#>#ifndef __UTILS_32H
#>#include <utils.h>
#>#endif
#>#undef select
#>#endif
#impinclude string
#impinclude sys/time
#impinclude SockException
#impinclude errno

//	Notes on select:
//		The thing about select() is, its not documented correctly.  Or at least
//	the documentation is misleading.  For example, in the EMX libarary docs,
// it says: "NDFDS [the first parameter of select] is the number of handles
//	to be checked".  Experimentation has shown that the first parameter should
// actually be the value of the highest handle plus one, that being the
// total number of low order bits to check.  For example, if you are
// interested in handles 3, 5, and 7, the first parameter of select()
// would be 8.

/*---------------------------------------------------------------------------*\
	The primary function of SockSet is to hide the obscenities of the
	"select()" function from Socket users.  This class allows you to
	easily maintain a set of sockets and wait for an event from any of them.
	
	It does not do memory management on any of its sockets, it is up to the
	caller to insure that they are correctly deleted.
\*---------------------------------------------------------------------------*/
class SockSet {

	public:

		enum SockEvent { read = 1, write = 2, except = 4 };
	
	private:

		struct SockWatch {
			const Socket *sock;
			int watchFor;		// should be a combination of read, write, except
			
			SockWatch(const Socket *sock0, int watchFor0) :
					sock(sock0),
					watchFor(watchFor0)
				{
				}
			
			SockWatch() :
					sock(0),
					watchFor(0)
				{
				}
			}
		
		SLList<SockWatch> elems, events;
		unsigned elemCount, elemSize;

		/*---------------------------------------------------------------------*\
			Finds a socket in the set with the matching handle.
		\*---------------------------------------------------------------------*/
		const Socket *findSocketWithHandle(int handle) const
			{
			for (Pix p = elems.first(); p; elems.next(p))
				if (elems(p).sock->handle() == handle)
					return elems(p).sock;
			return 0;
			}

		/*---------------------------------------------------------------------*\
			Does the actual low-level select call.  Fills the "events" queue
			based on the results.  'timeout' is in milliseconds.  A value
			of negative one (-1) indicates no timeout.
			Returns true if events were obtained, false on a timeout.
		\*---------------------------------------------------------------------*/
		bool select(long timeout)
			{
			int sockCount = 0;
			fd_set readSet, writeSet, exceptSet;
			FD_ZERO(&readSet);
			FD_ZERO(&writeSet);
			FD_ZERO(&exceptSet);
			
			// for each socket in the set, set the flags in the "fd_set"
			// bitmasks to indicate what kinds of events to wait for.
			for (Pix p = elems.first(); p; elems.next(p))
				{
				int h = elems(p).sock->handle();
				
				//  set all of the bits which are appropriate for this socket.
				if (elems(p).watchFor & read)
					FD_SET(h, &readSet);
				if (elems(p).watchFor & write)
					FD_SET(h, &writeSet);
				if (elems(p).watchFor & except)
					FD_SET(h, &exceptSet);
					
				// if the current handle is higher than the highest handle
				// so far, we have a new upper bound.
				if (h > sockCount - 1)
					sockCount = h + 1;
				}
			
			// do the low-level select with or without a timeout
			if (timeout != -1)
				{
				// got a timeout, construct a timeout structure from the
				// timeout in milliseconds
				struct timeval timeVal;
				timeVal.tv_sec = timeout / 1000;
				timeVal.tv_usec = (timeout % 1000) * 1000;

#ifndef __IBMCPP__
				sockCount = ::select(sockCount, &readSet, &writeSet, &exceptSet,
											&timeVal);
#else
				sockCount = ::bsdselect(sockCount, &readSet, &writeSet, &exceptSet,
												&timeVal);
#endif
				}
			else
				{
#ifndef __IBMCPP__
				sockCount = ::select(sockCount, &readSet, &writeSet, &exceptSet,
											0);
#else
				sockCount = ::bsdselect(sockCount, &readSet, &writeSet, &exceptSet,
												0);
#endif
				}

			if (sockCount < 0)
				// if an error occurred, throw an exception
				throw SockException(SockException::select,  errno);
			else if (!sockCount)
				// if we timed out,  stop now.
				return false;
			else
				{
				// walk through the result bitmaps,  for handle with a set
				// bit, create an event.
				for (int i = 0; sockCount; i++)
					{
					int evtType =
						(FD_ISSET(i, &readSet) ? read : 0) |
						(FD_ISSET(i, &writeSet) ? write : 0) |
						(FD_ISSET(i, &exceptSet) ? except : 0);
					if (evtType)
						{
						const Socket *sock = findSocketWithHandle(i);
						events.append(SockWatch(sock, evtType));
						sockCount--;
						}
					}
				return true;
				}
			}

	public:

		SockSet() :
				elems(),
				elemCount(0),
				elemSize(0)
			{
			}
		
		/*---------------------------------------------------------------------*\
			Adds a new socket to the set.  "watchFor" should be any combination
			of 'read', 'write', or 'except'.  'read' includes the events for
			connection on a ServerSocket.
		\*---------------------------------------------------------------------*/
		SockSet &add(const Socket *sock, int watchFor)
			{
			elems.append(SockWatch(sock, watchFor));
			return *this;
			}

		/*---------------------------------------------------------------------*\
			Removes an existing socket from the set.
		\*---------------------------------------------------------------------*/
		SockSet &remove(const Socket *sock)
			{
			Pix last = 0;
			
			// walk the list...
			for (Pix p = elems.first(); p; elems.next(p))
				// if we've found the matching element, delete it.  Since this
				// is a singly linked list, we can't delete the current element
				// so we have to keep track of the last element as well.
				if (elems(p).sock == sock)
					{
					if (last)
						elems.del_after(last);
					else
						elems.del_front();
					break;
					}
				else
					last = p;
			return *this;
			}
		
		/*---------------------------------------------------------------------*\
			Change the things to watch for on a particular socket.  "watchFor"
			should be any combination of 'read', 'write', or 'except'.
		\*---------------------------------------------------------------------*/
		SockSet &reset(const Socket *sock, int watchFor)
			{
			// walk the list...
			for (Pix p = elems.first(); p; elems.next(p))
				if (elems(p).sock == sock)
					{
					elems(p).watchFor = watchFor;
					break;
					}
			return *this;
			}

		/*---------------------------------------------------------------------*\
			Returns true if the SockSet includes sock.
		\*---------------------------------------------------------------------*/
		bool includes(const Socket *sock) const
			{
			for (Pix p = elems.first(); p; elems.next(p))
				if (elems(p).sock == sock)
					return true;
			return false;
			}

		/*---------------------------------------------------------------------*\
			Waits for the next event on any of the sockets in the set.
			"timeout" is specified in milliseconds,  with a value of -1
			being no timeout and a value of zero being no wait (poll the current
			state of all sockets in the set).
			Normally, "evt" is filled with the event type.
			If the call times out waiting for an event, it will return 0 and
			the value of "evt" is undefined.
		\*---------------------------------------------------------------------*/
		const Socket *nextEvent(SockSet::SockEvent &evt, long timeout = -1)
			{
			const Socket *rval;
			if (!events.length() && !select(timeout))
				return 0;

			if (!events.length())
				throw Exception("Bug in SockSet::nextEvent(): event list is empty.");

			rval = events(events.first()).sock;
			evt = (SockEvent)events(events.first()).watchFor;
			events.del_front();
			return rval;
			}
				
	}