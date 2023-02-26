#include "dsu.h"

#include <vector>

#include "gtest/gtest.h"

TEST(DsuTest, BasicTest) {
  Dsu dsu(6);
  dsu.Merge(0, 1);
  dsu.Merge(1, 2);
  dsu.Merge(3, 4);
  EXPECT_EQ(3, dsu.cnt);
  EXPECT_EQ(0, dsu.GetFa(2));
  EXPECT_EQ(std::vector<int>({3, 0, 0, 2, 0, 1}), dsu.GetCounts());
}