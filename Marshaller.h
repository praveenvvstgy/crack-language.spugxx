#ifndef SPUG_MARSHALLER_H
#define SPUG_MARSHALLER_H

#include "types.h"
#include <string>

namespace spug {

class Marshallable;

/**
 * Interface for asynchronous marshallers.  Asynchronous marshallers can be
 * used to serialize classes derived from Marshallable.
 *
 * The basic use of a Marshaller is as follows:
 * \code
 *    
 *    class SomeClass : public Marshallable {
 *  xxx finish documenting
 *  \endcode
 */
class Marshaller {

   public:
      enum { pause = -1 };

      virtual int write(const int &val) = 0;

      virtual int write(const size_t &val) = 0;

      virtual int write(const float &val) = 0;

      virtual int write(const std::string &str) = 0;

      virtual int write(const Marshallable &val) = 0;

      /**
       * This is a special variation on the write method in which "state" is a
       * direct index into the buffer.  It is used to implement primitive
       * writes.  Returns the new state.
       *
       * \param buffer the buffer
       * \param state  the state - an index into the buffer of the next byte
       *               to read
       * \param len    the total size of the buffer
       * \return the new state (a new index into the buffer).  If state ==
       *         len, the buffer was completely written
       */
      virtual int write(const Byte *buffer, int state, size_t len) = 0;

      virtual int read(int &val) = 0;

      virtual int read(size_t &val) = 0;

      virtual int read(float &val) = 0;

      virtual int read(std::string &val) = 0;

      virtual int read(Marshallable &val) = 0;

      /**
       * This is a special variation on the read method in which "state" is a
       * direct index into the buffer.  It is used to implement primitive
       * reads.  Returns the new state.
       *
       * \param buffer the buffer
       * \param state  the state - an index into the buffer of the next byte
       *               to read
       * \param len    the total size of the buffer
       * \return the new state (a new index into the buffer).  If state ==
       *         len, the buffer was completely read
       */
      virtual int read(Byte *buffer, int state, size_t len) = 0;

      /**
       * Marshalls the object (or part of it) into the buffer.  This is
       * intended to be called repeatedly until the object is fully
       * marshalled, at which point done() returns true.  "the object" must be
       * a marshallable object that has been associated with the Marshaller,
       * usually at construction.
       * \throw InvalidStateException Attempted to marshall the object after
       *                              marshalling is complete, or after
       *                              starting an unmarshall operation.
       */
      virtual size_t marshall(Byte *buffer, size_t size) = 0;

      /**
       * Unmarshalls the object (or part of it) into the buffer.  This is
       * intended to be called repeatedly until the object is fully
       * unmarshalled, at which point done() returns true.  "the object" must
       * be a marshallable object that has been associated with the
       * Marshaller, usually at construction.
       * \throw InvalidStateException Attempted to unmarshall the object after
       *                              unmarshalling is complete, or after
       *                              starting an marshall operation.
       */
      virtual size_t unmarshall(Byte *buffer, size_t size) = 0;

      /**
       * Returns true if the marshalling/unmarshalling of the object is
       * complete.
       */
      virtual bool done() const = 0;
};

} // namespace spug

#endif
