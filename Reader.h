/*===========================================================================*\

   $Id$

   Reader interface.

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

#ifndef SPUG_READER_H
#define SPUG_READER_H

#include <sys/types.h>
#include "types.h"

namespace spug {

/**
   Reader interface - base class of everything that can read a sequence of
   bytes.
 */
class Reader {

   public:
      /**
       * Reads 'count' bytes into buffer, or as many bytes are available,
       * depending on the nature of the implementation.
       */
      virtual size_t read(Byte *buffer, size_t bytes) = 0;
};

} // namespace spug

#endif
