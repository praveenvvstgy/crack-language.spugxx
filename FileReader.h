/*===========================================================================*\
     
    FileReader interface.

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
