#==============================================================================
#    
#   Copyright (C) 2008 Michael A. Muller
#
#   This file is part of spug++.
#
#   spug++ is free software: you can redistribute it and/or modify it under the 
#   terms of the GNU Lesser General Public License as published by the Free 
#   Software Foundation, either version 3 of the License, or (at your option) 
#   any later version.
#
#   spug++ is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License 
#   along with spug++.  If not, see <http://www.gnu.org/licenses/>.
#
#==============================================================================

## Process this file with automake to produce Makefile.in
noinst_PROGRAMS = tester
lib_LIBRARIES = libspug++.a
TESTS = tester
tester_SOURCES = tester.cc TestMarshaller.cc
tester_LDADD = libspug++.a
libspug___a_SOURCES = Tracer.cc FileReader.cc NativeMarshaller.cc \
   Marshallable.cc Socket.cc InetAddress.cc SockException.cc \
   SystemException.cc Mutex.cc Thread.cc Time.cc OwningByteBuf.cc
include_spugdir = $(prefix)/include/spug
include_spug_HEADERS = RCBase.h RCPtr.h Exception.h Tracer.h Reader.h \
   FileReader.h types.h Iterator.h IteratorImpl.h STLIteratorImpl.h casts.h \
   Marshallable.h Marshaller.h NativeMarshaller.h TypeInfo.h String.h \
   UnsupportedOperationException.h OutOfRangeException.h \
   Logger.h DoesNotExistException.h StringFmt.h SystemException.h Mutex.h \
   Locker.h InvalidStateException.h StronglyTypedScalar.h OwningByteBuf.h \
   ByteBuf.h Runnable.h Socket.h Thread.h Time.h TimeDelta.h InetAddress.h \
   SockException.h LPtr.h
noinst_HEADERS = TestMarshaller.h
EXTRA_DIST = testdata spug++.m4

install-autoconf:
	$(INSTALL) -d $(prefix)/share/aclocal
	$(INSTALL) spug++.m4 $(prefix)/share/aclocal

