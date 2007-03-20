## Process this file with automake to produce Makefile.in
noinst_PROGRAMS = tester
lib_LIBRARIES = libspug++.a
TESTS = tester
tester_SOURCES = tester.cc TestMarshaller.cc
tester_LDADD = libspug++.a
libspug___a_SOURCES = Tracer.cc FileReader.cc NativeMarshaller.cc \
   Marshallable.cc Socket.cc InetAddress.cc SockException.cc
include_spugdir = $(prefix)/include/spug
include_spug_HEADERS = RCBase.h RCPtr.h Exception.h Tracer.h Reader.h \
   FileReader.h types.h Iterator.h IteratorImpl.h STLIteratorImpl.h casts.h \
   Marshallable.h Marshaller.h NativeMarshaller.h TypeInfo.h String.h \
   UnsupportedOperationException.h OutOfRangeException.h \
   Logger.h DoesNotExistException.h

# XXX we apologize for the very lame release tarball generator

VER := $(shell svn info | awk '/^Revision:/ { print $$2; }')

tarball:
	svn export -r $(VER) $(svn)/trunk/spug++ spug++-$(VER)
	(cd spug++-$(VER); \
		rm *.d CntPtr.* PtrSList.h Ptrs.h mxfile Makefile.org; \
		./bootstrap)
	tar -cvzf spug++-$(VER).tar.gz spug++-$(VER)
	rm -rf spug++-$(VER)

install-autoconf:
	$(INSTALL) -d $(prefix)/share/aclocal
	$(INSTALL) spug++.m4 $(prefix)/share/aclocal
