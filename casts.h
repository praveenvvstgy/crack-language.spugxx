#ifndef SPUG_CASTS_H
#define SPUG_CASTS_H

namespace spug {

template <typename T, typename U>
inline T rcast(U val) { return reinterpret_cast<T>(val); }

template <typename T, typename U>
inline T dcast(U val) { return dynamic_cast<T>(val); }

template <typename T, typename U>
inline T scast(U val) { return static_cast<T>(val); }

template <typename T, typename U>
inline T ccast(U val) { return const_cast<T>(val); }

// cast away const and reinterpret - for those really hard to reach spots
template <typename T, typename U>
inline T rccast(const U val) { return reinterpret_cast<T>(const_cast<U>(val)); }

};

#endif
