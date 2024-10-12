#include "math/smawk.h"

#include "gtest/gtest.h"

TEST(SMAWKTest, BasicTest) {
  int n = 99;
  auto f = [](int i, int j) { return (i - j) * (i - j); };
  auto res = SMAWK(n, n, f);
  EXPECT_EQ(n, (int)res.size());
  for (int i = 0; i < n; i++) EXPECT_EQ(i, res[i]);
}