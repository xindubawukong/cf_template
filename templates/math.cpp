#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "debug.h"
#endif

using namespace std;

using float64 = double;
using float128 = long double;
using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

// --------------------------- xindubawukong ---------------------------


vector<int> GetPrimes(int n) {
  vector<int> primes;
  vector<bool> is_prime(n + 1, true);
  for (int i = 2; i <= n; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
    }
    for (int p: primes) {
      int x = p * i;
      if (x > n) break;
      is_prime[x] = false;
      if (i % p == 0) break;
    }
  }
  return primes;
}

void Main() {}

int main() {
#ifdef LOCAL
  freopen("../input.txt", "r", stdin);
  // freopen("../output.txt", "w", stdout);
#endif
  Main();
  return 0;
}
