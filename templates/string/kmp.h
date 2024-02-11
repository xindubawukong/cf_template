#ifndef KMP_H_
#define KMP_H_

#include <vector>

// pi[i]: max k such that s[0..k-1] = s[i-k+1..i]  (inclusive)
// z[i]: lcp of s[0..] and s[i..] (except z[0])
template <typename Seq>
auto KMP(const Seq& s) {
  int n = s.size();
  std::vector<int> pi(n);
  for (int i = 1, j = 0; i < n; i++) {
    while (j && s[j] != s[i]) j = pi[j - 1];
    if (s[j] == s[i]) j++;
    pi[i] = j;
  }
  std::vector<int> z(n);
  for (int i = 1, l = 0, r = 0; i < n; i++) {
    if (i <= r && z[i - l] < r - i + 1) {
      z[i] = z[i - l];
    } else {
      z[i] = std::max(0, r - i + 1);
      while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    }
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }
  return std::make_pair(pi, z);
}

#endif  // KMP_H_
