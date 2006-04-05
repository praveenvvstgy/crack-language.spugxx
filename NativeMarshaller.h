#ifndef SPUG_NATIVEMARSHALLER_H
#define SPUG_NATIVEMARSHALLER_H

#include <stack>
#include "types.h"
#include "casts.h"
#include "Marshaller.h"
#include "Marshallable.h"
#include "Tracer.h"

namespace spug {

/**
 * Built-in marshaller implementation.  Marshalls to binary streams.
 */
class NativeMarshaller : public Marshaller {

   private:

      // for tracking the current marshalling state - we keep a stack of these.
      struct Context {
         Marshallable &cur;
         int state;

         Context(Marshallable &cur, int state = 0) : cur(cur), state(state) {}
      };

      std::stack<Context> contexts;

      Byte *buffer;
      unsigned len;

      static Tracer::Level trace;

      /** 
       * Generic read implementation.  All of the read() methods are
       * essentially the same - they associate the primitive marshaller for
       * the type with the value and then push it onto the
       * context stack.  This method encapsulates that code into a template.
       */
      template <typename T, typename Mrsh>
      int genericRead(Marshaller &out, Mrsh &mrsh, T &val);

      /**
       * Generic write implementation.  Same idea as genericRead(), only for
       * writes.
       */
      template <typename T, typename Mrsh>
      int genericWrite(Marshaller &out, Mrsh &mrsh, const T &val);

      template <typename T>
      class PrimMarshallable : public Marshallable {
         private:
            T *valPtr;
            static Tracer::Level trace;

         public:

            PrimMarshallable() : valPtr(0) {}

            void setVal(T &val) {
               valPtr = &val;
            }

            virtual bool marshall(Marshaller &out, int state) const;
            virtual bool unmarshall(Marshaller &out, int state);
      };

      // class for marshalling arbitrary byte buffers
      class ByteBufMrsh : public Marshallable {
         private:
            static Tracer::Level trace;
         public:

            // data buffer and size
            Byte *data;
            size_t size;
            bool owns;

            ByteBufMrsh() : data(0), size(0) {}
            virtual ~ByteBufMrsh();

            virtual bool marshall(Marshaller &out, int state) const;
            virtual bool unmarshall(Marshaller &out, int state);
      };

      // class for marshalling std::string
      class StrMrsh : public Marshallable {
         private:
            std::string *valPtr;
            mutable ByteBufMrsh bbufMrsh;
            size_t tempBufferSize;

            // stores the string size during unmarshalling
            size_t size;

            static Tracer::Level trace;

         public:
            StrMrsh() : valPtr(0) {}

            void setVal(std::string &val) {
               valPtr = &val;
            }

            virtual bool marshall(Marshaller &out, int state) const;
            virtual bool unmarshall(Marshaller &out, int state);
      };
               
      typedef PrimMarshallable<int> IntMrsh;
      typedef PrimMarshallable<size_t> SizeTMrsh;
      typedef PrimMarshallable<float> FloatMrsh;

      IntMrsh intMrsh;
      SizeTMrsh sizeTMrsh;
      FloatMrsh floatMrsh;
      StrMrsh strMrsh;

   public:

      NativeMarshaller(Marshallable &objectToWrite);

      virtual int write(const int &val);
      virtual int write(const size_t &val);
      virtual int write(const float &val);
      virtual int write(const std::string &val);
      virtual int write(const Marshallable &val);
      virtual int write(const Byte *buffer, int state, size_t len);

      virtual int read(int &val);
      virtual int read(size_t &val);
      virtual int read(float &val);
      virtual int read(std::string &val);
      virtual int read(Marshallable &val);
      virtual int read(Byte *buffer, int state, size_t len);

      virtual size_t marshall(Byte *buffer, size_t size);
      virtual size_t unmarshall(Byte *buffer, size_t size);
      virtual bool done() const;

};

} // namespace spug

#endif
