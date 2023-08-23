#ifndef COMMON_H_
#define COMMON_H_

/*

Things that can directly put in the beginning.

*/

// clang-format off

using int64 = long long;
using uint = unsigned int;
using uint64 = unsigned long long;
template <typename T> bool ckmin(T& a, T b) { return b < a ? a = b, 1 : 0; }
template <typename T> bool ckmax(T& a, T b) { return b > a ? a = b, 1 : 0; }

// clang-format on

#endif  // COMMON_H_
