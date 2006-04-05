
#include "NativeMarshaller.h"

#include <string>
#include "TypeInfo.h"
#include "InvalidStateException.h"

using namespace spug;
using namespace std;

#define PM NativeMarshaller::PrimMarshallable<T>

namespace {

/**
 * A little manipulator to write up to 10 bytes of a buffer in hex & ascii
 * format.  e.g. the null terminated string "test" would be written 
 * [74 65 73 74 00] ("test.")
 */
class FmtBuf {
   private:
      Byte *data;
      size_t size;

   public:
      FmtBuf(Byte *data, size_t size) : data(data), size(size) {}

      friend ostream &operator <<(ostream &out, const FmtBuf &buf);
};

ostream &operator <<(ostream &out, const FmtBuf &buf) {

   size_t i;

   // write the bytes in hex

   out << '[';

   // write the first byte
   out << hex << (buf.data[0] >> 4) << hex << (buf.data[0] & 0xF);

   // write the rest of the bytes (up to 10 of them)
   for (i = 1; i < min(buf.size, 10u); ++i)
      out << ' ' << hex << (buf.data[i] >> 4) << hex << (buf.data[i] & 0xF);

   // if there were more than 10 bytes, add elipsis
   if (i < buf.size)
      out << "...";

   out << "] (\"";

   // write them as a string
   for (i = 0; i < min(buf.size, 10u); ++i) {
      Byte c = buf.data[i];
      if (c >= 32 && c <= 127 && c != '"' && c != '\\')
         cout << c;
      else if (c == '\\')
         cout << "\\\\";
      else if (c == '"')
         cout << "\\\"";
      else
         cout << "\\x" << (buf.data[i] >> 4) << hex << (buf.data[i] & 0xF);
   }

   out << "\")";
}

}

template <typename T>
Tracer::Level PM::trace = Tracer::get().getLevel("spug::NativeMarshaller");

template <typename T>
bool PM::marshall(Marshaller &out, int state) const {
   SPUG_TRACE(trace, 
              "marshalling " << TypeInfo::get(*this)->getName()
               << " with value " << *valPtr
               << " state = " << state << " to " 
               << FmtBuf(rcast<Byte *>(valPtr) + state, sizeof(T) - state)
              );

   return out.write(rcast<Byte *>(valPtr), state, sizeof(T)) == 
      sizeof(T);
}

template <typename T>
bool PM::unmarshall(Marshaller &out, int state) {
   SPUG_TRACE(trace, 
              "unmarshalling " << TypeInfo::get(*this)->getName()
               << " state = " << state
              );

   return out.read(rcast<Byte *>(valPtr), state, sizeof(T)) ==
      sizeof(T);
}

// implementation of byte buffer marshalling

Tracer::Level NativeMarshaller::ByteBufMrsh::trace = 
   Tracer::get().getLevel("spug::NativeMarshaller");

NativeMarshaller::ByteBufMrsh::~ByteBufMrsh() {
   if (owns)
      delete data;
}

bool NativeMarshaller::ByteBufMrsh::marshall(Marshaller &out, int state) const {
   SPUG_TRACE(trace, 
              "marshalling byte buffer, state = " << state << " size = "
               << size << " buffer = " 
               << FmtBuf(rcast<Byte *>(data) + state, size - state)
              );
   return out.write(data, state, size) == size;
}

bool NativeMarshaller::ByteBufMrsh::unmarshall(Marshaller &out, int state) {
   SPUG_TRACE(trace, 
              "unmarshalling byte buffer, state = " << state << " size = "
               << size
              );
   return out.read(data, state, size) == size;
}

// implementation of string marshalling

Tracer::Level NativeMarshaller::StrMrsh::trace = 
   Tracer::get().getLevel("spug::NativeMarshaller");

bool NativeMarshaller::StrMrsh::marshall(Marshaller &out, int state) const {
   SPUG_TRACE(trace, "marshalling string state = " << state);
   ccast<StrMrsh *>(this)->size = valPtr->size();
   while (state != Marshaller::pause)
      switch (state) {
         case 0:
            // in state zero, we write an integer
            state = out.write(size);
            break;
         case 1:
            // marshall the byte buffer
            bbufMrsh.data = rcast<Byte *>(ccast<char *>(valPtr->data()));
            bbufMrsh.size = size;
            bbufMrsh.owns = false;
            state = out.write(bbufMrsh);
            break;

         case 2:
            bbufMrsh.data = 0;
            return true;
      }

   return false;
}

bool NativeMarshaller::StrMrsh::unmarshall(Marshaller &out, int state) {
   SPUG_TRACE(trace, "unmarshalling string state = " << state);
   while (state != Marshaller::pause)
      switch (state) {
         case 0:
            // in state zero, we read an integer
            state = out.read(size);
            break;

         case 1:
            // read the string buffer

            // if we don't yet have a buffer, or it's not big enough, allocate one
            if (!bbufMrsh.data || bbufMrsh.size < size) {
               delete bbufMrsh.data;
               bbufMrsh.data = new Byte[size];
               bbufMrsh.size = size;
               bbufMrsh.owns = true;
            }

            // unmarshall the byte buffer
            state = out.read(bbufMrsh);
            break;
         case 2:
            *valPtr = string(rcast<char *>(bbufMrsh.data), size);
            return true;
            break;
      }

   return false;
}

Tracer::Level NativeMarshaller::trace = 
   Tracer::get().getLevel("spug::NativeMarshaller");

template <typename T, typename Mrsh>
inline int NativeMarshaller::genericRead(Marshaller &out, Mrsh &mrsh, T &val) {
   mrsh.setVal(val);
   contexts.push(Context(mrsh));
   if (mrsh.unmarshall(out, 0)) {
      contexts.pop();
      return ++(contexts.top().state);
   } else {
      return pause;
   }
}

template <typename T, typename Mrsh>
inline int 
NativeMarshaller::genericWrite(Marshaller &out, Mrsh &mrsh, const T &val) {
   mrsh.setVal(*ccast<T *>(&val));
   contexts.push(Context(mrsh));
   if (mrsh.marshall(out, 0)) {
      contexts.pop();
      return ++contexts.top().state;
   } else {
      return pause;
   }
}

NativeMarshaller::NativeMarshaller(Marshallable &objectToWrite) {
   contexts.push(Context(objectToWrite));
}

int NativeMarshaller::write(const int &val) {
   return genericWrite(*this, intMrsh, val);
}

int NativeMarshaller::write(const size_t &val) {
   return genericWrite(*this, sizeTMrsh, val);
}

int NativeMarshaller::write(const float &val) {
   return genericWrite(*this, floatMrsh, val);
}

int NativeMarshaller::write(const std::string &val) {
   return genericWrite(*this, strMrsh, val);
}

int NativeMarshaller::write(const Marshallable &val) {
   SPUG_TRACE(trace, "writing " << TypeInfo::get(val)->getName());

   // we can't use genericWrite() for this because the Marshallable _is_ the
   // value and it doesn't follow the same interface as the primitive
   // marshallers (no setVal() method).  Aside from this, the code is
   // basically the same.
   contexts.push(Context(*ccast<Marshallable *>(&val)));
   if (val.marshall(*this, 0)) {
      contexts.pop();
      return ++contexts.top().state;
   } else {
      return pause;
   }
}

int NativeMarshaller::write(const Byte *data, int state, size_t size) {
   size_t remaining = size - state;

   // if the buffer is big enough to accomodate the rest of the data, write it
   // all
   if (len >= remaining) {
      memcpy(buffer, &data[state], remaining);
      len -= remaining;
      buffer += remaining;

      // store this as the new state
      contexts.top().state = size;

      return size;
   } else {
      // write what we can
      size_t endState = state + len;
      memcpy(buffer, &data[state], len);
      buffer += len;
      len = 0;

      // store this as the new state
      contexts.top().state = endState;

      return endState;
   }
}

int NativeMarshaller::read(int &val) {
   return genericRead(*this, intMrsh, val);
}

int NativeMarshaller::read(size_t &val) {
   return genericRead(*this, sizeTMrsh, val);
}

int NativeMarshaller::read(float &val) {
   return genericRead(*this, floatMrsh, val);
}

int NativeMarshaller::read(std::string &val) {
   return genericRead(*this, strMrsh, val);
}

int NativeMarshaller::read(Marshallable &val) {
   SPUG_TRACE(trace, "reading " << TypeInfo::get(val)->getName());

   // see write(Marshallable&).  Same basic idea, only in reverse.
   contexts.push(Context(val));
   if (val.unmarshall(*this, 0)) {
      contexts.pop();
      return ++contexts.top().state;
   } else {
      return pause;
   }
}

int NativeMarshaller::read(Byte *readBuffer, int state, size_t size) {
   size_t remaining = size - state;
   SPUG_TRACE(trace, "reading up to " << remaining 
               << " bytes into buffer from "
               << FmtBuf(buffer, len)
              );

   // if the buffer has enough data to accomodate the request, read it all
   if (len >= remaining) {
      memcpy(&readBuffer[state], buffer, remaining);
      len -= remaining;
      buffer += remaining;

      // store this as the new state
      contexts.top().state = size;

      return size;
   } else {
      // read what we can
      int endState = state + len;
      memcpy(&readBuffer[state], buffer, len);
      buffer += len;
      len = 0;

      // store this as the new state
      contexts.top().state = endState;

      return endState;
   }
}

size_t NativeMarshaller::marshall(Byte *destBuffer, size_t size) {
   if (!contexts.size())
      throw InvalidStateException("Attempt to marshall an object that is "
                                    "already fully marshalled."
                                  );
   SPUG_TRACE(trace, 
              "marshalling, top object is " 
               << TypeInfo::get(contexts.top().cur)->getName()
               << ", state = " << contexts.top().state
              );

   buffer = destBuffer;
   len = size;
   if (contexts.top().cur.marshall(*this, contexts.top().state)) {
      contexts.pop();
      if (contexts.size())
         ++contexts.top().state;
   }

   // number of bytes written is the original buffer size minus the current
   // buffer size
   return size - len;
}

size_t NativeMarshaller::unmarshall(Byte *srcBuffer, size_t size) {
   if (!contexts.size())
      throw InvalidStateException("Attempt to unmarshall an object that is "
                                    "already fully unmarshalled."
                                  );
   SPUG_TRACE(trace, 
              "unmarshalling, top object is " 
               << TypeInfo::get(contexts.top().cur)->getName()
               << ", state = " << contexts.top().state
              );

   buffer = srcBuffer;
   len = size;
   if (contexts.top().cur.unmarshall(*this, contexts.top().state)) {
      contexts.pop();
      if (contexts.size())
         ++contexts.top().state;
   }

   return size - len;
}

bool NativeMarshaller::done() const {
   return !contexts.size();
}
