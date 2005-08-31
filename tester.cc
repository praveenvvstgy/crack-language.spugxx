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
#include <iostream>
#include <fstream>

using namespace std;
using namespace spug;

ofstream logger("tester.log");

void success() {
   cerr << "ok" << endl;
   logger << "ok" << endl;
}

void failed() {
   cerr << "failed" << endl;
   logger << "failed" << endl;
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
      rcptr_func(tester);
   END_TEST(true)

   BEGIN_TEST("assignment to base class");
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
      RCPtr<RCBase> base;
      base = tester;
   END_TEST(tester->refcnt() == 2)

   BEGIN_TEST("construction of base class pointer");
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
      // XXX for some reason, this doesn't work if we use the equivalent "base
      // = tester" form...
      RCPtr<RCBase> base(tester);
   END_TEST(tester->refcnt() == 2)
}

