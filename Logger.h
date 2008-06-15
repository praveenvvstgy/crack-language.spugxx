/*===========================================================================*\
     
    Placeholder for real logging functions. See also the "Tracer" module, 
    which provides more detailed function tracing.
    
    Copyright (C) 2007 Michael A. Muller
 
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

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#define SPUG_WARN(msg) std::cerr << msg << std::endl;
#define SPUG_ERROR(msg) std::cerr << msg << std::endl;
#define SPUG_NOTICE(msg) std::cerr << msg << std::endl;

#endif
