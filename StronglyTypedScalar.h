/*===========================================================================*\
     
    Copyright (C) 2007 Michael A. Muller
 
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
