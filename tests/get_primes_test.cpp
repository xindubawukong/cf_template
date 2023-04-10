#include "get_primes.h"

#include <vector>

#include "gtest/gtest.h"

TEST(GetPrimesTest, BasicTest) {
  auto [primes, is_prime, minp] = GetPrimes(20);
  EXPECT_EQ(primes, std::vector<int>({2, 3, 5, 7, 11, 13, 17, 19}));
  for (int i = 0; i <= 20; i++) {
    if (std::find(primes.begin(), primes.end(), i) == primes.end()) {
      EXPECT_FALSE(is_prime[i]);
    } else {
      EXPECT_TRUE(is_prime[i]);
    }
  }
  EXPECT_EQ(minp, std::vector<int>({0,  0, 2,  3, 2, 5, 2,  7, 2,  3, 2,
                                    11, 2, 13, 2, 3, 2, 17, 2, 19, 2}));
}