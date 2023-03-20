#include "get_primes.h"

#include <vector>

#include "gtest/gtest.h"

TEST(GetPrimesTest, BasicTest) {
  EXPECT_EQ(std::vector<int>({2, 3, 5, 7, 11, 13, 17, 19}), GetPrimes(20));
}