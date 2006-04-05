#ifndef MARSHALLTEST_H
#define MARSHALLTEST_H

#include "Marshallable.h"
#include <string>

class TestMarshaller : public spug::Marshallable {

   public:
      int intVal;
      float floatVal;
      std::string stringVal;

      TestMarshaller() : intVal(0), floatVal(0) {}

      TestMarshaller(int intVal, float floatVal, const std::string &stringVal) :
         intVal(intVal),
         floatVal(floatVal),
         stringVal(stringVal) {
      }

      virtual bool marshall(spug::Marshaller &out, int state) const;

      virtual bool unmarshall(spug::Marshaller &out, int state);
};

#endif
