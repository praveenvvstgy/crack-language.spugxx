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
#include "LPtr.h"
#include "Exception.h"
#include "Tracer.h"
#include "FileReader.h"
#include "Iterator.h"
#include "STLIteratorImpl.h"
#include "TestMarshaller.h"
#include "NativeMarshaller.h"
#include "Socket.h"
#include "TypeInfo.h"
#include "StringFmt.h"
#include "Mutex.h"
#include "Locker.h"
#include "SystemException.h"
#include "OwningByteBuf.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

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

SPUG_EXCEPTION(Fail)

#define BEGIN_TEST(title) \
   cerr << title << "..." << flush; \
   logger << "=============" << "\n" << title << "\n" << "===========" << endl; \
   try {

#define END_TEST(expr) \
   if (expr) \
      success(); \
   else \
      failed(); \
   } catch (const Fail &ex) { \
      logger << ex << endl; \
      failed(); \
   } catch (const Exception &ex) { \
      logger << ex << endl; \
      failed(); \
   } catch (const exception &ex) { \
      logger << ex.what() << endl; \
      failed(); \
   } catch (...) { \
      logger << "anon exception" << endl; \
      failed(); \
   }

#define FAIL(msg) throw Fail(msg);

class Tester : public RCBase {
   public:
      bool &deleted;
      Tester(bool &deleted) : deleted(deleted) {}

      ~Tester() { deleted = true; }
};

class NotTester : public RCBase {};

void rcptr_func(const RCPtr<RCBase> &obj) {}

typedef STLIteratorImpl<char, vector<char>, vector<char>::iterator> 
   CharIterImpl;

class TestCharIterImpl : public CharIterImpl {
   public:
      bool destructed;

      TestCharIterImpl(vector<char> &coll) : 
	 CharIterImpl(coll), 
	 destructed(false) {
      }

      ~TestCharIterImpl() {
	 destructed = true;
      }
};

// override global delete
typedef std::map<void *, bool> FreeMap;
FreeMap *freeMap = 0;
void operator delete(void *ptr) {
   // XXX should probably either delete _or_ store, then cleanup later.
   if (freeMap) {
      (*freeMap)[ptr] = true;
   }
   free(ptr);
}

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
   END_TEST(temp.str() == "spug::Exception: basic string")

   BEGIN_TEST("exception from string object")
      string str = "basic string";
      Exception ex(str);
      ostringstream temp;
      temp << ex;
   END_TEST(temp.str() == "spug::Exception: basic string")

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

   BEGIN_TEST("FileReader::seek()")
      Byte buf[5];
      FileReader reader("testdata");
      reader.seek(5);
   END_TEST(reader.read(buf, 5) == 5 &&
	    std::string(reinterpret_cast<const char *>(buf), 5) == "67890"
	    )


   BEGIN_TEST("Basic iterators")
      vector<char> v;
      char buf[5];
      v.push_back('t');
      v.push_back('e');
      v.push_back('s');
      v.push_back('t');
      int i = 0;
      for (Iterator<char> ci(CharIterImpl::alloc(v)); ci; ++ci, ++i)
	 buf[i] = *ci;
      buf[i] = 0;
   END_TEST(std::string(buf) == "test")

#if 0
   BEGIN_TEST("iterator cleanup")
      vector<char> v;
      {
	 Iterator<char> iter(TestCharIterImpl::alloc
#endif

   BEGIN_TEST("TypeInfo construction from instance and demangling")
      FileReader reader("testdata");
   END_TEST(TypeInfo::get(reader)->getName() == std::string("spug::FileReader"))

   {
      Byte buffer[100];
      const size_t bufSize = sizeof(buffer);
      BEGIN_TEST("Marshalling")
         TestMarshaller writeable(100, 1.0, "test");
         NativeMarshaller m(writeable);
         size_t count = m.marshall(buffer, sizeof(buffer));
         FILE *fp = fopen("marshalled.out", "w");
         fwrite(buffer, 1, count, fp);
         fclose(fp);
      END_TEST(count < sizeof(buffer))

      BEGIN_TEST("Unmarshalling")
         TestMarshaller readable;
         NativeMarshaller m(readable);
         size_t count = m.unmarshall(buffer, sizeof(buffer));
      END_TEST(readable.intVal == 100 && readable.floatVal == 1.0 &&
               readable.stringVal == "test"
               )
   }

#if 0
   Tracer &tracer = Tracer::get();
   tracer.enable(tracer.getLevel("spug::NativeMarshaller"));
#endif
   BEGIN_TEST("Marshalling across buffers")
      int totalSize = 100;
      for (size_t bufSize = 1; bufSize < totalSize; ++bufSize) {
         Byte *buffer = new Byte[bufSize + 4];
         int beef = *rccast<int *, char *>("BEEF");

         // stick a marker at the end of the buffer
         *rcast<int *>(&buffer[bufSize]) = beef;

         TestMarshaller readable(100, 1.0, "test"), writable;
         NativeMarshaller m(readable), n(writable);
         while (!m.done()) {
            // marshall from one
            size_t count = m.marshall(buffer, bufSize);
            if (count > bufSize)
               FAIL("marshalled too much");

            if (*rcast<int *>(&buffer[bufSize]) != beef)
               FAIL("bad beef!");

            // unmarshall to the other
            if (n.unmarshall(buffer, bufSize) > bufSize)
               FAIL("unmarshalled too much");
         }

         if (!n.done())
            FAIL("done marshalling, but not done unmarshalling");

         if (writable.intVal != 100 || writable.floatVal != 1.0 || 
             writable.stringVal != "test"
             )
            FAIL("unmarshalled to an incorrect state");

         delete buffer;
      }
   END_TEST(true)

   BEGIN_TEST("basic socket functionality")
      Socket srv(0);
      srv.listen(5);
      Socket client = Socket(InetAddress("127.0.0.1"), srv.getPort());
      Socket serverClient = srv.accept();
      char outBuf[] = "this is a test";
      client.send(outBuf, sizeof(outBuf));
      char inBuf[256];
      serverClient.recv(inBuf, sizeof(inBuf));
      strcmp(inBuf, outBuf);
      if (strcmp(inBuf, outBuf))
         FAIL("buffers do not match");
   END_TEST(true)

   BEGIN_TEST("string formatting")
   END_TEST(SPUG_FSTR("this is " << "a test") == "this is a test")

   BEGIN_TEST("useless mutex test")
      Mutex m;
      m.acquire();
      m.release();
   END_TEST(true)

   BEGIN_TEST("system exceptions")
      SystemException ex(0, "test message");
      std::string temp = "test message: ";
      temp += strerror(0);
   END_TEST(temp == ex.getMessage())

   BEGIN_TEST("mutex locker")
      Mutex m(false);
      {
         Locker lk(m);
      }
      // will hang if we haven't released
      m.acquire();
      m.release();
   END_TEST(true)
   
   BEGIN_TEST("OwningByteBuf")
      freeMap = new FreeMap;
      Byte *allocated = new Byte[10];
      Byte copied[6] = "hello";
      {
         OwningByteBuf buf = OwningByteBuf::wrap(10, allocated);
         OwningByteBuf copiedBuf(sizeof(copied), copied);
         if (memcmp(copiedBuf.buffer, copied, sizeof(copied)) ||
             copiedBuf.size != sizeof(copied))
             FAIL("Copied buffer differs from original");
      }
   END_TEST(freeMap->find(allocated) != freeMap->end() &&
            freeMap->find(copied) == freeMap->end())
   freeMap = 0;

   BEGIN_TEST("Basic LPtrs")
      bool deleted = false;
      {
	 LPtr<Tester> a = new Tester(deleted);

	 // just verify that we can access members
	 a->deleted;

	 // verify that we can dererference
	 (*a).deleted;

	 LPtr<Tester> b = a;
	 a = b;

	 // implicit assignment to a raw pointer
	 Tester *raw = a;

	 // pass through the raw pointer to another LPtr
	 LPtr<Tester> c = raw;
      }
   END_TEST(deleted)

#if 0
      assert(m.done());
      Unmarshaller u(readable);
      m.write(buffer, sizeof(buffer));
      assert(readable.intVal == 100 && readable.stringVal == "test");
#endif

   return failCount > 0;
}

