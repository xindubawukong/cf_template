#ifndef HASH_STRING_H_
#define HASH_STRING_H_

#include <vector>

template <int P1, int M1, int P2, int M2>
struct DoubleHashString {
  int n;
  std::vector<int> h1, h2;

  HashString(const string& s) {
    n = s.length();
    h1.resize(n);
    h2.resize(n);
    h1[0] = h2[0] = s[0] - 'a' + 1;
    for (int i = 1; i < n; i++) {
      h1[i] = (long long)h1[i - 1] * P1 % M1;
      h2[i] = (long long)h2[i - 1] * P2 % M2;
    }
  }

  std::pair<int, int> GetHash(int l, int r) const {
    assert(0 <= l && l <= r && r < n);
    int a = 0, b = 0;
    for (int i = 0; i < hash_config.size(); i++) {
      int mod = hash_config[i].second;
      int a = hash[i][r];
      int b = (int64)(l == 0 ? 0 : hash[i][l - 1]) * fact[i][r - l + 1] % mod;
      res[i] = (a - b + mod) % mod;
    }
    return res;
  }

  int Length() const { return hash[0].size(); }

  static void Prepare(int n) {
    fact.resize(hash_config.size());
    for (int i = 0; i < fact.size(); i++) {
      fact[i].resize(n + 1);
      auto [p, mod] = hash_config[i];
      fact[i][0] = 1;
      for (int j = 1; j <= n; j++) {
        fact[i][j] = (int64)fact[i][j - 1] * p % mod;
      }
    }
  }
};

#endif  // HASH_STRING_H_
