/*===========================================================================*\

   $Id$

   FileReader interface.

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

#ifndef SPUG_FILEREADER_H
#define SPUG_FILEREADER_H

#include <sys/types.h>
#include <unistd.h>
#include "Reader.h"

namespace spug {

/**
 * Class for reading bytes from a file.
 */
class FileReader : public Reader {

   private:
      int fd;

   public:

      /** Constructs an instance by opening the named file. */
      FileReader(const char *name);

      ~FileReader();

      virtual size_t read(Byte *buffer, size_t count);

      /** close the file. */
      void close();

      enum Whence { 
	 seekSet = SEEK_SET, 
	 seekCur = SEEK_CUR, 
	 seekEnd = SEEK_END 
      };

      /**
       * Seeks to a file position.
       */
      off_t seek(off_t offset, Whence whence = seekSet);
};

} // namespace spug
#endif
