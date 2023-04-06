#include "dsu.h"

#include <random>
#include <vector>

#include "debug.h"
#include "gtest/gtest.h"

TEST(DsuTest, BasicTest) {
  Dsu dsu(6);
  dsu.Unite(0, 1);
  dsu.Unite(1, 2);
  dsu.Unite(3, 4);
  EXPECT_EQ(3, dsu.cnt);
  EXPECT_EQ(dsu.Find(0), dsu.Find(2));

  Dsu<false> dsu2(6);
  dsu2.Unite(0, 1);
  dsu2.Unite(1, 2);
  dsu2.Unite(3, 4);
  EXPECT_EQ(3, dsu2.cnt);
  EXPECT_EQ(dsu2.Find(0), dsu2.Find(2));
}

TEST(DsuTest, UndoTest) {
  Dsu<false, true> dsu(6);
  dsu.Unite(0, 1);
  dsu.Unite(2, 3);
  dsu.Unite(3, 4);
  dsu.Unite(1, 5);
  EXPECT_EQ(dsu.Find(0), dsu.Find(5));
  dsu.Undo(1);
  EXPECT_EQ(dsu.Find(0), dsu.Find(1));
  EXPECT_NE(dsu.Find(0), dsu.Find(5));
  EXPECT_EQ(dsu.Find(2), dsu.Find(4));
  dsu.Undo(1);
  EXPECT_NE(dsu.Find(2), dsu.Find(4));
  dsu.Undo(dsu.undo.size());
  for (int i = 0; i < 6; i++) {
    EXPECT_EQ(dsu.Find(i), i);
    EXPECT_EQ(1, dsu.size[i]);
  }
}