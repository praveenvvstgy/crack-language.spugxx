
#ifndef SPUG_EXCEPTION_H
#define SPUG_EXCEPTION_H

#include <exception>
#include <string>
#include <iostream>

namespace spug {

/**
 * Simple exception base class that allows exceptions to be thrown with an
 * informative message.
 */
class Exception : public std::exception {

   private:
      std::string msg;

   public:
      Exception(const char *msg) : msg(msg) {}
      Exception(const std::string &msg) : msg(msg) {}

      ~Exception() throw () {}

      virtual const char *getClassName() const { return "Exception"; }

      friend std::ostream &
	 operator <<(std::ostream &out, const Exception &err);
};

inline std::ostream &operator <<(std::ostream &out, const Exception &err) {
   out << err.getClassName() << ": " << err.msg;
}

// some macros to make it extremely easy to define derived exceptions

// Defines an exception class derived from an arbitrary base class
#define SPUG_DERIVED_EXCEPTION(cls, base) \
   class cls : public base { \
      public: \
	 cls(const char *msg) : base(msg) {} \
	 cls(const std::string &msg) : base(msg) {} \
	 virtual const char *getClassName() const { return #cls; } \
   };

// defines an exception class derived from spug::Exception
#define SPUG_EXCEPTION(cls) SPUG_DERIVED_EXCEPTION(cls, spug::Exception)

}

#endif

