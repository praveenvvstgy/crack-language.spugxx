

GNULIB_INCLUDE=-I../gnulib

ifndef MCC
MCC=mcc
endif

.SUFFIXES: .d .h.new

.d.cc:
	$(MCC) $<

.cc.o:
	g++ -g -c -I. $(GNULIB_INCLUDE) $< -o $*.o

%.h : %.h.new
	{\
	if [ ! -e $*.h ] || ! diff $*.h $*.h.new >/dev/null ;\
	then\
		mv $*.h.new $*.h;\
	fi;\
	}

# source files
SRCS:=\
	Exception.d\
	CntObj.d\
	CntPtrBase.d\
	InetAddress.d\
	MsgBase.d\
	MsgId.d\
	MsgKeySet.d\
	MsgList.d\
	MsgNumber.d\
	MsgString.d\
	PSLBase.d\
	ServerSocket.d\
	SockException.d\
	SockSet.d\
	SockStream.d\
	Socket.d\

	

ALL:
	$(MAKE) srcs
	$(MAKE) headers
	$(MAKE) libmlib.a

srcs: $(SRCS:.d=.cc)

headers: $(SRCS:.d=.h)

libmlib.a: $(subst .d,.o,$(SRCS))
	ar rcv $@ $?

clean:
	rm -f *.o
	rm -f libmlib.a
	rm -f make.out
	rm -f $(SRCS:.d=.h)
	rm -f $(SRCS:.d=.h.new)
	rm -f $(SRCS:.d=.cc)
	rm -f *~
