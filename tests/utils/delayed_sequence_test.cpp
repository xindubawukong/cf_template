#include "utils/delayed_sequence.h"

#include <algorithm>
#include <numeric>
#include <vector>

#include "gtest/gtest.h"

TEST(DelayedSequenceTest, IteratesGeneratedValues) {
  DelayedSequence seq(5, [](size_t i) { return static_cast<int>(i * i); });

  std::vector<int> values(seq.begin(), seq.end());
  EXPECT_EQ(values, std::vector<int>({0, 1, 4, 9, 16}));
  EXPECT_EQ(std::accumulate(seq.begin(), seq.end(), 0), 30);
  EXPECT_EQ(*std::max_element(seq.begin(), seq.end()), 16);
}

TEST(DelayedSequenceTest, SupportsRandomAccessIteratorOperations) {
  DelayedSequence seq(3, 7, [](size_t i) { return static_cast<int>(i * 2); });

  auto it = seq.begin();
  EXPECT_EQ(*it, 6);
  EXPECT_EQ(it[2], 10);
  EXPECT_EQ(*(it + 3), 12);
  EXPECT_EQ(seq.end() - seq.begin(), 4);

  it += 2;
  EXPECT_EQ(*it, 10);
  EXPECT_EQ(it - seq.begin(), 2);
  EXPECT_TRUE(seq.begin() < it);

  std::vector<int> reversed(seq.rbegin(), seq.rend());
  EXPECT_EQ(reversed, std::vector<int>({12, 10, 8, 6}));
}
