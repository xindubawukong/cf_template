#include "suffix_array.h"

#include <random>
#include <string>

#include "gtest/gtest.h"

std::mt19937
    rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST(SuffixArrayTest, PrefixDoublingTest) {
  int n = rng() % 500 + 200;
  std::string s;
  s.resize(n);
  for (int i = 0; i < n; i++) {
    s[i] = 'a' + rng() % 3;
  }
  auto [sa, rank, lcp] = SuffixArrayPrefixDoubling(s);
  std::vector<std::string> a(n);
  for (int i = 0; i < n; i++) {
    a[i] = s.substr(i, n - i);
  }
  sort(a.begin(), a.end());
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(sa[i], n - a[i].size());
  }
}
