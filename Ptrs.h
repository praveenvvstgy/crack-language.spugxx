#ifndef Ptrs_H
#define Ptrs_H

template <class T>
class ArrPtr {
   private:
      T *p;

      // verboten!
//      ArrPtr(const ArrPtr &other);
//      ArrPtr &operator =(const ArrPtr &other);

   public:

#ifdef CONFORMS_APR_95
		// use the explicit keyword if it exists
      explicit
#endif
		ArrPtr(T *p0 = 0) : p(p0) {}
      ArrPtr(ArrPtr<T> &other)
         { p = other.p; other.p = 0; }
      ArrPtr<T> &operator =(ArrPtr<T> &other)
         { p = other.p; other.p = 0; return *this; }
      ArrPtr<T> &operator =(T *p0)
         { p = p0; return *this; return *this; }
      ~ArrPtr() { if (p) delete [] p; }
      T *operator ->() const { return p; }
      T &operator *() const { return *p; }
      operator T *() const { return p; }
      T *release() { T *q = p; p = 0; return q; }
   };

template <class T>
class ObjPtr {
   private:
      T *p;

      // verboten!
//      ObjPtr(const ObjPtr &other);
//      ObjPtr &operator =(const ObjPtr &other);      

   public:
#ifdef CONFORMS_APR_95
		// use the explicit keyword if it exists
      explicit
#endif
      ObjPtr(T *p0 = 0) : p(p0) {}
      ObjPtr(ObjPtr<T> &other)
         { p = other.p; other.p = 0; }
      ObjPtr<T> &operator =(ObjPtr<T> &other)
         { p = other.p; other.p = 0; return *this; }
      ObjPtr<T> &operator =(T *p0)
         { p = p0; return *this; }
      ~ObjPtr() { if (p) delete p; }
      T *operator ->() const { return p; }
      T &operator *() const { return *p; }
      operator T*() const { return p; }
      T *release() { T *q = p; p = 0; return q; }
   };

#endif
