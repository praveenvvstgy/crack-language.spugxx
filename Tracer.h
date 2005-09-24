/*===========================================================================*\

   Interface for configurable tracing.

   Copyright (C) 2005 Michael A. Muller

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
      static Tracer theTracer;

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
      static Tracer &get() { return theTracer; }

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
   if (Tracer::get().enabled(level)) cout << msg << endl;

#endif
