#ifndef GET_PRIMES_H_
#define GET_PRIMES_H_

#include <vector>

template <typename T>
std::vector<T> GetPrimes(T n) {
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
  return primes;
}

#endif  // GET_PRIMES_H_