/*===========================================================================*\

   $Id$

   FileReader implementation.

   Copyright (C) 2006 Michael A. Muller

   Permission is granted to use, modify and redistribute this code,
   providing that the following conditions are met:
 
   1) This copyright/licensing notice must remain intact.
   2) If the code is modified and redistributed, the modifications must 
   include documentation indicating that the code has been modified.
   3) The author(s) of this code must be indemnified and held harmless
   against any damage resulting from the use of this code.

   This code comes with ABSOLUTELY NO WARRANTEE, not even the implied 
   warrantee of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

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
