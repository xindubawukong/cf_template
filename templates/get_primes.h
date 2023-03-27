#ifndef GET_PRIMES_H_
#define GET_PRIMES_H_

#include <vector>

template <typename T>
std::pair<std::vector<T>, std::vector<bool>> GetPrimes(T n) {
  std::vector<T> primes;
  std::vector<bool> is_prime(n + 1, true);
  for (T i = 2; i <= n; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
    }
    for (T p : primes) {
      T x = p * i;
      if (x > n) break;
      is_prime[x] = false;
      if (i % p == 0) break;
    }
  }
  return std::make_pair(primes, is_prime);
}

#endif  // GET_PRIMES_H_