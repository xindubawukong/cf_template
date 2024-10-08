#ifndef HASH_STRING_H_
#define HASH_STRING_H_

#include <cassert>
#include <string>
#include <vector>

template <int P0 = 29, int M0 = 1000000007, int P1 = 31, int M1 = 1000000009>
struct DoubleHashingHelper {
  static_assert(P0 > 0 && M0 > 0 && P1 > 0 && M1 > 0);
  struct HashString {
    DoubleHashingHelper* helper;
    int n;
    std::vector<int> a0, a1;
    HashString() : n(0) {}
    HashString(HashString&& h)
        : helper(h.helper), n(h.n), a0(std::move(h.a0)), a1(std::move(h.a1)) {}

    std::pair<int, int> Hash() { return {a0[n - 1], a1[n - 1]}; }

    std::pair<int, int> Hash(int l, int r) {
      assert(0 <= l && l <= r && r < n);
      int a, b, h0, h1;
      a = a0[r];
      b = (long long)(l == 0 ? 0 : a0[l - 1]) * helper->f0[r - l + 1] % M0;
      h0 = (a - b + M0) % M0;
      a = a1[r];
      b = (long long)(l == 0 ? 0 : a1[l - 1]) * helper->f1[r - l + 1] % M1;
      h1 = (a - b + M1) % M1;
      return {h0, h1};
    }
  };
  int maxn;
  std::vector<int> f0, f1;
  DoubleHashingHelper() {
    maxn = 0;
    f0 = f1 = {1};
  }
  void ExpandTo(int n) {
    if (n <= maxn) return;
    f0.resize(n + 1);
    f1.resize(n + 1);
    for (int i = maxn + 1; i <= n; i++) {
      f0[i] = (long long)f0[i - 1] * P0 % M0;
      f1[i] = (long long)f1[i - 1] * P1 % M1;
    }
    maxn = n;
  }
  HashString Create(const std::string& s) {
    ExpandTo(s.length());
    HashString hh;
    hh.n = s.length();
    assert(hh.n <= maxn);
    hh.a0.resize(hh.n);
    hh.a1.resize(hh.n);
    hh.a0[0] = hh.a1[0] = s[0];
    for (int i = 1; i < hh.n; i++) {
      hh.a0[i] = ((long long)hh.a0[i - 1] * P0 + s[i]) % M0;
      hh.a1[i] = ((long long)hh.a1[i - 1] * P1 + s[i]) % M1;
    }
    hh.helper = this;
    return hh;
  }
};

#endif  // HASH_STRING_H_
