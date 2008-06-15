/*===========================================================================*\

    Interface for configurable tracing.
     
    Copyright (C) 2005 Michael A. Muller
 
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

#ifndef SPUG_TRACER_H
#define SPUG_TRACER_H

#include <map>
#include <vector>

namespace spug {

/**
 * Manages tracing (writing trace information from the code.
 *
 * The Tracer singleton includes a set of trace levels.  The trace levels are
 * flags which each indicate whether a given category of trace messages are
 * logged.
 */
class Tracer {
   private:
      typedef std::map<std::string, int> NameMap;
      NameMap levelByName;
      std::vector<bool> levels;
      int lastLevel;

      // the tracer singleton
      static Tracer *theTracer;

      Tracer();

   public:

      /** 
       * A trace level - this is really just an integer, but its value is
       * opaque to the application.  It is constructed with the "getLevel()"
       * method.
       */
      class Level {
	 friend class Tracer;
	 private:
	    int index;
	    Level(int index) : index(index) {}
      };

      /** returns the Tracer singleton */
      static Tracer &get();

      /** Returns a Level identifier for a given level name. */
      Level getLevel(const char *levelName);

      /** Returns true if the level is enabled for tracing */
      bool enabled(Level level) const {
	 return levels[level.index];
      }

      /** Enables the trace level */
      void enable(Level level) {
	 levels[level.index] = true;
      }

      /** Disables the trace level */
      void disable(Level level) {
	 levels[level.index] = false;
      }

      /** 
       * Enables or disables the trace level, depending on the value of
       * "enabled".
       */
      void setLevel(Level level, bool enabled) {
	 levels[level.index] = enabled;
      }

};

} // namespace spug

#define SPUG_TRACE(level, msg) \
   if (spug::Tracer::get().enabled(level)) std::cout << msg << std::endl;

#endif
