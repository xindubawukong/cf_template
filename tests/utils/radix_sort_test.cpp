#include "utils/radix_sort.h"

#include <vector>

#include "gtest/gtest.h"

TEST(RadixSortTest, BasicTest) {
  std::vector<int> a = {4, 1, 2, 5, 3};
  RadixSort(a, 5, [](int i) { return i; });
  EXPECT_EQ(a, std::vector<int>({1, 2, 3, 4, 5}));

  a = {1, 2, 3, 4, 0};
  std::vector<int> b = {2, 2, 0, 4, 6};
  RadixSort(a, 6, [&](int i) { return b[i]; });
  EXPECT_EQ(a, std::vector<int>({2, 1, 0, 3, 4}));
}
