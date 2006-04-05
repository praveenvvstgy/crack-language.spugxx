#ifndef SPUG_MARSHALLABLE_H
#define SPUG_MARSHALLABLE_H

namespace spug {

class Marshaller;

/**
 * Abstract base class for all objects that can be marsrhalled and
 * unmarshalled using Marshaller.
 */
class Marshallable {
   public:

      virtual ~Marshallable();

      /**
       * Marshalls the object to the given marshaller.  Returns true if the
       * object is completely marshalled, false if not.
       */
      virtual bool marshall(Marshaller &marshaller, int state) const = 0;

      /**
       * Unmarshalls the object from "unmarshaller".  Returns true if the
       * object is completely unmarshalled, false if not.
       */
      virtual bool unmarshall(Marshaller &marshaller, int state) = 0;
};

} // namespace spug
#endif
