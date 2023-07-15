#include "string/hash_string.h"

#include <random>
#include <string>

#include "gtest/gtest.h"

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST(HashStringTest, BasicTest) {
  int n = 1;
  std::string s;
  for (int i = 0; i < n; i++) s += rng() % 26 + 'a';
  s += s;
  std::cout << s << '\n';
  DoubleHashingHelper<29, 1000000007, 31, 1000000009> helper;
  auto hs = helper.Create(s);
  EXPECT_EQ(hs.Hash(0, n - 1), hs.Hash(n, 2 * n - 1));
}