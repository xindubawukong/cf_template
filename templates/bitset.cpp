#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "../debug.h"
#else
#define debug(...) 19980723;
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

mt19937 rng(19980723);
mt19937_64 rng64(19980723);

// --------------------------- xindubawukong ---------------------------

template <uint n>
struct BitSet {
  uint len;
  vector<uint64> a;

  BitSet(uint64 x = 0) {
    len = n / 64;
    if (n % 64) len++;
    a.resize(len);
    for (int i = 0; i < len; i++) a[i] = 0;
    a[0] = x;
  }

  operator string() const {
    string s;
    for (int i = len - 1; i >= 0; i--) {
      string t = bitset<64>(a[i]).to_string();
      if (i == len - 1) {
        t = t.substr(64 - n % 64);
      }
      s += t;
    }
    return s;
  }

  void Set(uint k, uint val) {
    assert(k < n);
    assert(val == 0 || val == 1);
    auto& x = a[k / 64];
    auto p = k % 64;
    x ^= x & ((uint64)1 << p);
    x ^= (uint64)val << p;
  }

  uint Get(uint k) const {
    assert(k < n);
    auto& x = a[k / 64];
    auto p = k % 64;
    return (x >> p) & 1;
  }

  uint FindFirst() const {
    for (int i = 0; i < len; i++) {
      if (a[i]) {
        for (int j = 0; j < 64; j++) {
          if ((a[i] >> j) & 1) {
            return 64u * i + j;
          }
        }
        assert(false);
      }
    }
    return n;
  }

  void operator&=(const BitSet<n>& b) {
    for (int i = 0; i < len; i++) {
      a[i] &= b.a[i];
    }
  }

  friend BitSet<n> operator&(const BitSet<n>& a, const BitSet<n>& b) {
    BitSet<n> c = a;
    c &= b;
    return c;
  }
};

void Main() {}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
#ifdef LOCAL
  freopen("../problem_A/A.in", "r", stdin);
  // freopen("../problem_A/A.out", "w", stdout);
#endif
  Main();
  return 0;
}
