#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "debug.h"
#endif

using namespace std;

using float64 = long double;
using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

// --------------------------- xindubawukong ---------------------------

template <typename T, T base>
class BigInt {
  static_assert(base > 0);

 public:
  vector<T> a;
  BigInt(T x = 0) : a({x}) { Normalize(); }
  BigInt(const BigInt<T, base>& b) : a(b.a) {}

  operator string() const {
    string t = to_string(base - 1);
    string s;
    for (int i = a.size() - 1; i >= 0; i--) {
      string now = to_string(a[i]);
      if (i < a.size() - 1) {
        while (now.length() < t.length()) now = '0' + now;
      }
      s += now;
    }
    return s;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigInt<T, base>& a) {
    os << "BigInt(" << static_cast<string>(a) << ")";
    return os;
  }

  friend std::istream& operator>>(std::istream& is, BigInt<T, base>& a) {
    static_assert(base == 10 || base == 100 || base == 1000 || base == 10000 ||
                  base == 100000 || base == 1000000);
    string s;
    is >> s;
    int len = to_string(base - 1).length();
    std::reverse(s.begin(), s.end());
    a.a.clear();
    for (int i = 0; i < s.length(); i += len) {
      T x = 0;
      for (int j = min((int)s.length() - 1, i + len - 1); j >= i; j--) {
        x = x * 10 + s[j] - '0';
      }
      a.a.push_back(x);
    }
    a.Normalize();
    return is;
  }

  void Normalize() {
    for (int i = 0; i < a.size(); i++) {
      if (a[i] >= base) {
        if (i == a.size() - 1) a.push_back(0);
        a[i + 1] += a[i] / base;
        a[i] %= base;
      }
      while (a[i] < 0) {
        assert(i < a.size() - 1);
        a[i] += base;
        a[i + 1]--;
      }
    }
    while (a.size() > 1 && a.back() == 0) a.pop_back();
  }

  bool operator<(const BigInt<T, base>& b) const {
    int n = a.size(), m = b.a.size();
    if (n < m) return true;
    if (n > m) return false;
    for (int i = n - 1; i >= 0; i--) {
      if (a[i] < b.a[i]) return true;
      if (a[i] > b.a[i]) return false;
    }
    return false;
  }

  bool operator<=(const BigInt<T, base>& b) const { return !(b < *this); }

  bool operator>(const BigInt<T, base>& b) const { return b < *this; }

  bool operator>=(const BigInt<T, base>& b) const { return b <= *this; }

  bool operator==(const BigInt<T, base>& b) const {
    if (a.size() != b.a.size()) return false;
    for (int i = 0; i < a.size(); i++) {
      if (a[i] != b.a[i]) return false;
    }
    return true;
  }

  bool operator!=(const BigInt<T, base>& b) const { return !(*this == b); }

  BigInt operator+(const BigInt<T, base>& b) const {
    int n = a.size(), m = b.a.size();
    int len = max(n, m);
    BigInt c;
    c.a.resize(len);
    for (int i = 0; i < len; i++) {
      c.a[i] = (i < n ? a[i] : 0) + (i < m ? b.a[i] : 0);
    }
    c.Normalize();
    return c;
  }

  BigInt operator-(const BigInt<T, base>& b) const {
    int m = b.a.size();
    BigInt c = *this;
    assert(c >= b);
    for (int i = 0; i < m; i++) {
      c.a[i] -= b.a[i];
    }
    c.Normalize();
    return c;
  }

  BigInt operator*(int x) const {
    int n = a.size();
    auto b = *this;
    for (int i = 0; i < n; i++) {
      b.a[i] *= x;
    }
    b.Normalize();
    return b;
  }

  friend BigInt operator*(int x, const BigInt<T, base>& a) { return a * x; }

  BigInt operator*(const BigInt<T, base>& b) const {
    int n = a.size(), m = b.a.size();
    BigInt c;
    c.a.resize(n + m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        c.a[i + j] += a[i] * b.a[j];
      }
    }
    c.Normalize();
    return c;
  }

  // return a/b and a%b
  pair<BigInt, BigInt> operator/(const BigInt<T, base>& b) const {
    BigInt ans = *this, rest = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
      rest = rest * base + a[i];
      T x;
      if (rest >= b) {
        x = 1;
        // find max x that b * x <= rest
        T l = 2, r = base;
        while (l <= r) {
          T m = (l + r) / 2;
          if (b * m <= rest) {
            x = m;
            l = m + 1;
          } else {
            r = m - 1;
          }
        }
      } else {
        x = 0;
      }
      ans.a[i] = x;
      rest = rest - b * x;
    }
    ans.Normalize();
    return make_pair(ans, rest);
  }
};

using bigint = BigInt<int, 1000>;

void Main() {
  BigInt<int, 100> p = 12345678;
  BigInt<int, 100> q = 766;
  auto [x, y] = p / q;
  debug(x);
  debug(y);
}

int main() {
#ifdef LOCAL
  freopen("../input.txt", "r", stdin);
  // freopen("../output.txt", "w", stdout);
#endif
  Main();
  return 0;
}
