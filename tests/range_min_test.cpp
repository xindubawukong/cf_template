#include "range_min.h"

#include <iostream>
#include <random>

#include "gtest/gtest.h"

using namespace std;

int GetMaxIndex(const vector<unsigned int>& a, int l, int r) {
  int res = l;
  for (int i = l + 1; i <= r; i++) {
    if (a[i] > a[res]) res = i;
  }
  return res;
}

TEST(RangeMinTest, RandomTest) {
  std::mt19937 rng(
      chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = rng() % 1000 + 10;
  auto cmp = [](unsigned int x, unsigned int y) { return x > y; };
  vector<unsigned int> a(n);
  for (int i = 0; i < n; i++) a[i] = rng();
  RMQ<unsigned int, decltype(cmp)> rmq(a);
  int iter = 1000;
  for (int i = 0; i < iter; i++) {
    int l = rng() % n, r = rng() % n;
    if (l > r) swap(l, r);
    EXPECT_EQ(rmq.Query(l, r), GetMaxIndex(a, l, r));
  }
}
