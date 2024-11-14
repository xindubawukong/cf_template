#include "gtest/gtest.h"
#include "math/smawk.h"

TEST(SMAWKTest, BasicTest) {
  int n = 99;
  auto f = [](int i, int j) { return (i - j) * (i - j); };
  auto res = SMAWK_RowMin(n, n, f);
  EXPECT_EQ(n, (int)res.size());
  for (int i = 0; i < n; i++) EXPECT_EQ(i, res[i]);
}