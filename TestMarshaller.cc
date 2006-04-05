
#include "TestMarshaller.h"
#include "Marshaller.h"
#include "types.h"

using namespace spug;

bool TestMarshaller::marshall(Marshaller &out, int state) const {
   while (state != Marshaller::pause)
      switch (state) {
         case 0:
            state = out.write(intVal);
            break;
         case 1:
            state = out.write(floatVal);
            break;
         case 2:
            state = out.write(stringVal);
            break;
         case 3:
            return true;
      }

   return false;
}

bool TestMarshaller::unmarshall(Marshaller &out, int state) {
   while (state != Marshaller::pause)
      switch (state) {
         case 0:
            state = out.read(intVal);
            break;
         case 1:
            state = out.read(floatVal);
            break;
         case 2:
            state = out.read(stringVal);
            break;
         case 3:
            return true;
      }

   return false;
}
