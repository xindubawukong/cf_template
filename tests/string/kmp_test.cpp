#include "string/kmp.h"

#include <random>
#include <vector>

#include "gtest/gtest.h"

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST(KMPTest, BasicTest) {
  int n = 100;
  std::vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = rng() % 2;
  auto [pi, z] = KMP(a);
  for (int i = 0; i < n; i++) {
    int t = i;
    while (t > 0) {
      bool ok = true;
      for (int j = 0; j < t; j++) {
        if (a[j] != a[i - t + j + 1]) ok = false;
      }
      if (ok) break;
      t--;
    }
    EXPECT_EQ(pi[i], t);
    t = 0;
    while (i + t < n && a[t] == a[i + t]) t++;
    if (i == 0) t = 0;
    EXPECT_EQ(z[i], t);
  }
}