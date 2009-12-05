/*===========================================================================*\

    Unit test program for spug++ objects
     
    Copyright (C) 2005 Michael A. Muller
 
    This file is part of spug++.
 
    spug++ is free software: you can redistribute it and/or modify it under the 
    terms of the GNU Lesser General Public License as published by the Free 
    Software Foundation, either version 3 of the License, or (at your option) 
    any later version.
 
    spug++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public License 
    along with spug++.  If not, see <http://www.gnu.org/licenses/>.
 
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
#include "Condition.h"
#include "NativeMarshaller.h"
#include "Socket.h"
#include "Thread.h"
#include "Time.h"
#include "TimeDelta.h"
#include "TypeInfo.h"
#include "Reactable.h"
#include "Reactor.h"
#include "Runnable.h"
#include "StringFmt.h"
#include "Mutex.h"
#include "Locker.h"
#include "SystemException.h"
#include "OwningByteBuf.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <errno.h>

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
#define ASSERT_EQUALS(expect, actual) \
   if ((expect) != (actual)) throw Fail(#expect " != " #actual);

class Tester : public RCBase {
   public:
      bool &deleted;
      Tester(bool &deleted) : deleted(deleted) {}

      ~Tester() { deleted = true; }
};

class NotTester : public RCBase {};

void rcptr_func(RCBase *obj) {}

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

struct ThreadTester : public Runnable {
   bool val;
   
   ThreadTester() : val(false) {}
      
   virtual void run() {
      val = true;
   }
};

SPUG_LPTR(ThreadTester);
         
struct TestReactable : public Reactable {

   int fd;   
   string lastRead;
   string nextWrite;
   Reactable::Status status;
   Condition cond;
   
   TestReactable(int fileno, Reactable::Status status) : 
      fd(fileno), 
      status(status) {
   }

   virtual Reactable::Status getStatus() { return status; }

   virtual int fileno() {
      return fd;
   }

   virtual void handleRead(Reactor &reactor) {
      char buffer[1024];
      int rc = ::read(fd, buffer, 1024);
      if (rc < 0) {
         throw SystemException(errno, "read");
      } else if (rc == 0) {
         handleDisconnect(reactor);
         return;
      }
      ConditionLocker locker(cond);
      lastRead = string(buffer, rc);
      cond.notify();
   }
   
   virtual void handleWrite(Reactor &reactor) {
      ConditionLocker locker(cond);
      ::write(fd, nextWrite.data(), nextWrite.size());
   }
   
   virtual void handleDisconnect(Reactor &reactor) {
      reactor.removeReactable(this);
      fd = 0;
   }
   
   virtual void handleError(Reactor &reactor) {
      logger << "handleError called." << endl;
   }
   
};

SPUG_LPTR(TestReactable);  

struct TestReactableThread : public Runnable {
   TestReactableLPtr reactable;
   RunnableLPtr event;

   TestReactableThread(TestReactable *reactable, Runnable *event) : 
      reactable(reactable),
      event(event) {
   }

   virtual void run() {
      ReactorLPtr reactor = Reactor::createReactor();
      reactor->addReactable(reactable);
      reactor->schedule(TimeDelta(0, 100000), event);
      reactor->run();
   }
};   

struct Event : public Runnable {
   bool triggered;
   Condition cond;
   
   Event() : triggered(false) {}

   virtual void run() {
      ConditionLocker lock(cond);
      triggered = true;
      cond.notify();
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
      rcptr_func(tester.get());
   END_TEST(true)

   BEGIN_TEST("assignment to base class");
      bool deleted;
      RCPtr<Tester> tester = new Tester(deleted);
      RCPtr<RCBase> base;
      base = tester;
//      base = tester;
   END_TEST(tester->refcnt() == 2)

   BEGIN_TEST("dynamic cast to derived class");
     bool deleted;
     RCPtr<RCBase> base = new Tester(deleted);
     Tester *tester = RCPtr<Tester>::rcast(base);
   END_TEST(tester && tester->refcnt() == 1)

   BEGIN_TEST("failing dynamic cast");
     bool deleted;
     RCPtr<RCBase> base = new Tester(deleted);
     NotTester *notTester = RCPtr<NotTester>::rcast(base);
   END_TEST(!notTester)

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
      Byte assigned[6] = "hello";
      Byte assignedFromBase[19] = "assigned from base";
      strcpy((char *)assigned, "hello");
      {
         OwningByteBuf buf = OwningByteBuf::wrap(10, allocated);
         OwningByteBuf copiedBuf(sizeof(copied), copied);
         if (memcmp(copiedBuf.buffer, copied, sizeof(copied)) ||
             copiedBuf.size != sizeof(copied))
             FAIL("Copied buffer differs from original");
             
         OwningByteBuf assignedBuf;
         OwningByteBuf rval(sizeof(assigned), assigned);
         assignedBuf = rval;
         if (memcmp(assignedBuf.buffer, assigned, rval.size))
            FAIL("Assigned buffer differs from original");
            
         OwningByteBuf assignedFromBaseBuf;
         OwningByteBuf rval2(sizeof(assignedFromBase), assignedFromBase);
         assignedFromBaseBuf = rval2;
         if (memcmp(assignedFromBaseBuf.buffer, assignedFromBase, rval.size))
            FAIL("Assigned buffer differs from original");
      }
   END_TEST(freeMap->find(allocated) != freeMap->end() &&
            freeMap->find(copied) == freeMap->end() &&
            freeMap->find(assigned) == freeMap->end() &&
            freeMap->find(assignedFromBase) == freeMap->end())
   freeMap = 0;
   
   BEGIN_TEST("OwningByteBuf default constructor")
      char block[] = "failfailfail";
      OwningByteBuf *buf = new(block) OwningByteBuf();
   END_TEST(buf->buffer == 0)
   
   BEGIN_TEST("OwningByteBuf assignment.")
      Byte *wrapped = new Byte[10];
      OwningByteBuf buf;
      buf.assign(10, wrapped);
   END_TEST(buf.buffer == wrapped)

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
   
   BEGIN_TEST("LPtr assigning to existing object")
      bool deleted = false;
      {
         LPtr<Tester> a = new Tester(deleted);
	 // verify we don't delete when reassigning the same object
	 a = a;
	 if (deleted)
	     FAIL("Deleted on assignment to self");
      }
   END_TEST(deleted)
   
   BEGIN_TEST("TimeDelta")
      TimeDelta td1(100, 200);
      ASSERT_EQUALS(td1.getSeconds(), 100)
      ASSERT_EQUALS(td1.getMicroseconds(), 200)

      TimeDelta td2(100);
      ASSERT_EQUALS(td2.getSeconds(), 100)
      ASSERT_EQUALS(td2.getMicroseconds(), 0)
      
      // test normalizations
      ASSERT_EQUALS(TimeDelta(0, 1000000), TimeDelta(1, 0))
      ASSERT_EQUALS(TimeDelta(0, -1000000), TimeDelta(-1, 0))
      ASSERT_EQUALS(TimeDelta(1, -500000), TimeDelta(0, 500000))
      ASSERT_EQUALS(TimeDelta(-1, 500000), TimeDelta(0, -500000))
      
      // test addition and subtraction
      ASSERT_EQUALS(td1 - td2, TimeDelta(0, 200))
      ASSERT_EQUALS(td1 + td2, TimeDelta(200, 200))
      ASSERT_EQUALS(td2 - td1, TimeDelta(0, -200))
      ASSERT_EQUALS(TimeDelta(1, 1234).toMillis(), 1001)
      
      // test streaming
      ASSERT_EQUALS(SPUG_FSTR(TimeDelta(1, 0)), "1.000000");
      ASSERT_EQUALS(SPUG_FSTR(TimeDelta(-1, 0)), "-1.000000");
      ASSERT_EQUALS(SPUG_FSTR(TimeDelta(-1, -100000)), "-1.100000");
      ASSERT_EQUALS(SPUG_FSTR(TimeDelta(1, 100000)), "1.100000");
      
      Time t = Time::now();
      // can't test now() without dependency injection...
   END_TEST(true)   
   
   BEGIN_TEST("Thread")
      ThreadTesterLPtr tester = new ThreadTester();
      Thread thread(tester);
      thread.incref();
      thread.start();
      thread.join();
      ASSERT_EQUALS(tester->val, true);
   END_TEST(true)

   BEGIN_TEST("Reactor")
   
      // create a pipe
      int fds[2];
      ASSERT_EQUALS(pipe(fds), 0);
      
      LPtr<Event> event = new Event();

      // create the reactable and runnable
      TestReactableLPtr reactable = 
         new TestReactable(fds[0], Reactable::readyToRead);
      TestReactableThread runnable(reactable, event);
      runnable.incref();

      // create and start the reactor thread
      Thread thread(&runnable);
      thread.incref();
      thread.start();
      
      // guarantee that the write end of the pipe gets closed no matter what 
      // happens.
      struct Closer {
         int fd;
         Closer(int fd) : fd(fd) {}
         ~Closer() {
            close(fd);
         }
      } closer(fds[1]);
      
      write(fds[1], "read data", 9);
      {
         ConditionLocker locker(reactable->cond);
         if (reactable->lastRead != "read data")
            ASSERT_EQUALS(reactable->cond.wait(TimeDelta(1)), true);
         ASSERT_EQUALS(reactable->lastRead, "read data");
      }
      
      // wait for the scheduled event.
      {
         ConditionLocker locker(event->cond);
         if (!event->triggered)
            ASSERT_EQUALS(event->cond.wait(TimeDelta(1)), true);
         ASSERT_EQUALS(event->triggered, true);
      }

   END_TEST(true)
      

#if 0
      assert(m.done());
      Unmarshaller u(readable);
      m.write(buffer, sizeof(buffer));
      assert(readable.intVal == 100 && readable.stringVal == "test");
#endif

   return failCount > 0;
}


