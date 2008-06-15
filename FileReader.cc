/*===========================================================================*\
     
    FileReader implementation.

    Copyright (C) 2006 Michael A. Muller
 
    This file is part of spug++.
 
    spug++ is free software: you can redistribute it and/or modify it under the 
    terms of the GNU Lesser General Public License as published by the Free 
    Software Foundation, either version 3 of the License, or (at your option) 
    any later version.
 
    spug++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public License 
    along with spug++.  If not, see <http://www.gnu.org/licenses/>.
 
\*===========================================================================*/

#include "FileReader.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "Exception.h"

using namespace spug;

namespace {
   void throwSysError() {
      char errbuf[Const::errorBufferSize];
      strerror_r(errno, errbuf, Const::errorBufferSize);
      throw Exception(errbuf);
   }
}

FileReader::FileReader(const char *name) {
   fd = open(name, O_RDONLY);

   // XXX should throw a SystemError or more specific derivative here
   if (fd == -1)
      throwSysError();
}

FileReader::~FileReader() {
   if (fd != -1)
      close();
}

size_t FileReader::read(Byte *buffer, size_t count) {
   size_t rval = ::read(fd, buffer, count);
   if (rval == -1)
      throwSysError();
   return rval;
}

void FileReader::close() {
   ::close(fd);
   fd = -1;
}

off_t FileReader::seek(off_t offset, FileReader::Whence whence) {

// translate the flag if necessary - shouldn't be an issue
#if SEEK_SET != 0 || SEEK_CUR != 1 || SEEK_END != 2
   switch (whence) {
      case seekSet:
	 whence = SEEK_SET;
	 break;
      case seekCur:
	 whence = SEEK_CUR;
	 break;
      case seekEnd:
	 whence = SEEK_END;
	 break;
   }
#endif

   return lseek(fd, offset, whence);
}
