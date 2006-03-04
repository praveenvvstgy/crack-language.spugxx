/*===========================================================================*\

   Unit test program for spug++ objects

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

#include "RCPtr.h"
#include "RCBase.h"
#include "Exception.h"
#include "Tracer.h"
#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace spug;

ofstream logger("tester.log");

int failCount = 0;

void success() {
   cerr << "ok" << endl;
   logger << "ok" << endl;
}

void failed() {
   cerr << "failed" << endl;
   logger << "failed" << endl;
   ++failCount;
}

#define BEGIN_TEST(title) \
   cerr << title << "..." << flush; \
   logger << "=============" << "\n" << title << "\n" << "===========" << endl; \
   try {

#define END_TEST(expr) \
   if (expr) \
      success(); \
   else \
      failed(); \
   } catch (const exception &ex) { \
      logger << ex.what() << endl; \
      failed(); \
   } catch (...) { \
      logger << "anon exception" << endl; \
      failed(); \
   }

class Tester : public RCBase {
   public:
      bool &deleted;
      Tester(bool &deleted) : deleted(deleted) {}

      ~Tester() { deleted = true; }
};

class NotTester : public RCBase {};

void rcptr_func(const RCPtr<RCBase> &obj) {}

main() {
   RCBase obj;
   obj.incref();

   BEGIN_TEST("RCPtr construction")
      RCPtr<RCBase> p = &obj;
   END_TEST(obj.refcnt() == 2)

   BEGIN_TEST("RCPtr destruction")
   END_TEST(obj.refcnt() == 1);

   BEGIN_TEST("Object destruction")
      bool deleted = false;
      {
	 RCPtr<Tester> p = new Tester(deleted);
      }
   END_TEST(deleted)

   BEGIN_TEST("passing as base class")
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
//      rcptr_func(tester);
      rcptr_func(RCPtr<RCBase>::ucast(tester));
   END_TEST(true)

   BEGIN_TEST("assignment to base class");
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
      RCPtr<RCBase> base;
      base = RCPtr<RCBase>::ucast(tester);
//      base = tester;
   END_TEST(tester->refcnt() == 2)

   BEGIN_TEST("construction of base class pointer");
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
      // XXX for some reason, this doesn't work if we use the equivalent "base
      // = tester" form...
      RCPtr<RCBase> base(RCPtr<RCBase>::ucast(tester));
   END_TEST(tester->refcnt() == 2)

   BEGIN_TEST("dynamic cast to derived class");
     bool deleted;
     RCPtr<RCBase> base = new Tester(deleted);
     RCPtr<Tester> tester = RCPtr<Tester>::dcast(base);
   END_TEST(tester->refcnt() == 2)

   BEGIN_TEST("failing dynamic cast");
      bool passed = false;
      try {
	bool deleted;
	RCPtr<RCBase> base = new Tester(deleted);
	RCPtr<NotTester> notTester = RCPtr<NotTester>::dcast(base);
      } catch (const bad_cast &ex) {
	 passed = true;
      }
   END_TEST(passed)

   BEGIN_TEST("basic exception construction and streaming");
      Exception ex("basic string");
      ostringstream temp;
      temp << ex;
   END_TEST(temp.str() == "Exception: basic string")

   BEGIN_TEST("exception from string object")
      string str = "basic string";
      Exception ex(str);
      ostringstream temp;
      temp << ex;
   END_TEST(temp.str() == "Exception: basic string")

   Tracer::Level a = Tracer::get().getLevel("a");
   Tracer::Level b = Tracer::get().getLevel("b");

   BEGIN_TEST("enabling a level")
      Tracer::get().enable(a);
   END_TEST(true)

   BEGIN_TEST("checking enabled level")
   END_TEST(Tracer::get().enabled(a));

   BEGIN_TEST("checking disabled level")
   END_TEST(!Tracer::get().enabled(b));

   BEGIN_TEST("checking FileReader")
      Byte buf[80];
      FileReader reader("testdata");
   END_TEST(reader.read(buf, sizeof(buf)) == sizeof(buf) &&
	    std::string(reinterpret_cast<const char *>(buf), 80) == 
	     std::string("1234567890123456789012345678901234567890"
		          "1234567890123456789012345678901234567890"
			 )
	    )

   return failCount > 0;
}

