#include "math/shawk.h"

#include "gtest/gtest.h"

TEST(SHAWKTest, BasicTest) {
  int n = 99;
  auto f = [](int i, int j) { return (i - j) * (i - j); };
  auto res = SHAWK(n, n, f);
  EXPECT_EQ(n, (int)res.size());
  for (int i = 0; i < n; i++) EXPECT_EQ(i, res[i]);
}