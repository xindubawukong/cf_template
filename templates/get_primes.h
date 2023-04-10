#ifndef GET_PRIMES_H_
#define GET_PRIMES_H_

#include <vector>

template <typename T = int>
std::tuple<std::vector<T>, std::vector<bool>, std::vector<T>> GetPrimes(T n) {
  std::vector<T> primes;
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  std::vector<T> minp(n + 1);
  for (T i = 2; i <= n; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
      minp[i] = i;
    }
    for (T p : primes) {
      T x = p * i;
      if (x > n) break;
      is_prime[x] = false;
      minp[x] = p;
      if (i % p == 0) break;
    }
  }
  return {primes, is_prime, minp};
}

#endif  // GET_PRIMES_H_