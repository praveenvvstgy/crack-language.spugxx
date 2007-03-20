/*===========================================================================*\

   $Id$

   Placeholder for real logging functions. See also the "Tracer" module, which 
   provides more detailed function tracing.

   Copyright (C) 2007 Michael A. Muller

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

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#define SPUG_WARN(msg) std::cerr << msg << std::endl;
#define SPUG_ERROR(msg) std::cerr << msg << std::endl;
#define SPUG_NOTICE(msg) std::cerr << msg << std::endl;

#endif
