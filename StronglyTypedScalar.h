/*===========================================================================*\

   Copyright (C) 2007 Michael A. Muller

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

#ifndef SPUG_STRONGLYTYPEDSCALAR_H
#define SPUG_STRONGLYTYPEDSCALAR_H

namespace spug {

/**
 * Use this template to represent scalars in a typesafe manner.
 */
template <class T>
class StronglyTypedScalar {
   private:
      T val;

   public:
      StronglyTypedScalar() : val(0) {}
      explicit StronglyTypedScalar(T val) : val(val) {}

      StronglyTypedScalar operator +(StronglyTypedScalar other) {
         return StronglyTypedScalar(val + other.val);
      }

      StronglyTypedScalar operator -(StronglyTypedScalar other) {
         return StronglyTypedScalar(val - other.val);
      }

      StronglyTypedScalar operator /(StronglyTypedScalar other) {
         return StronglyTypedScalar(val / other.val);
      }

      StronglyTypedScalar operator *(StronglyTypedScalar other) {
         return StronglyTypedScalar(val * other.val);
      }

      StronglyTypedScalar &operator +=(StronglyTypedScalar other) {
         val += other.val;
         return *this;
      }

      StronglyTypedScalar &operator -=(StronglyTypedScalar other) {
         val -= other.val;
         return *this;
      }

      StronglyTypedScalar &operator *=(StronglyTypedScalar other) {
         val += other.val;
         return *this;
      }

      operator T() { return val; }
};

} // namespace spug

#endif
