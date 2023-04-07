#include "dsu.h"

#include <random>
#include <vector>

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

TEST(DsuTest, RelationTest) {
  auto plus = [](int x, int y) { return x ^ y; };
  auto negate = [](int x) { return x; };
  int n = 10;
  DsuWithRelation<true, false, int, decltype(plus), decltype(negate)> dsu(
      n * 2, plus, negate);
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  for (int i = 0; i < 30; i++) {
    int x = rng() % n, y = rng() % n + n;
    int rx, ry, tx, ty;
    std::tie(rx, tx) = dsu.Find(x);
    std::tie(ry, ty) = dsu.Find(y);
    if (rx != ry) {
      dsu.Unite(x, y, 1);
    } else {
      EXPECT_NE(tx, ty);
    }
  }
}

TEST(DsuTest, RelationUndoTest) {
  auto plus = [](int x, int y) { return x ^ y; };
  auto negate = [](int x) { return x; };
  DsuWithRelation<false, true, int, decltype(plus), decltype(negate)> dsu(
      4, plus, negate);
  dsu.Unite(0, 2, 1);
  dsu.Unite(0, 3, 1);
  EXPECT_EQ(dsu.Find(2), dsu.Find(3));
  dsu.Undo(1);
  dsu.Unite(2, 3, 1);
  EXPECT_NE(dsu.Find(2), dsu.Find(3));
}
