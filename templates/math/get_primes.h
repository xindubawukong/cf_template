#ifndef GET_PRIMES_H_
#define GET_PRIMES_H_

#include <tuple>
#include <vector>

template <typename T = int>
auto GetPrimes(int n) {
  std::vector<int> primes;
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  std::vector<int> minp(n + 1);
  for (int i = 2; i <= n; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
      minp[i] = i;
    }
    for (int p : primes) {
      int x = p * i;
      if (x > n) break;
      is_prime[x] = false;
      minp[x] = p;
      if (i % p == 0) break;
    }
  }
  return std::make_tuple(primes, is_prime, minp);
}

#endif  // GET_PRIMES_H_