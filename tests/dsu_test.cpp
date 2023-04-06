#include "dsu.h"

#include <vector>

#include "gtest/gtest.h"

TEST(DsuTest, BasicTest) {
  Dsu dsu(6);
  dsu.Unite(0, 1);
  dsu.Unite(1, 2);
  dsu.Unite(3, 4);
  EXPECT_EQ(3, dsu.cnt);
  EXPECT_EQ(0, dsu.Find(2));
}