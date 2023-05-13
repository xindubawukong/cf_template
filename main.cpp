#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef LOCAL
#include "debug.h"
#include "parlay/internal/get_time.h"
using timer = parlay::internal::timer;
#else
#define debug(...) 19980723
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
using uint128 = __uint128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

// mt19937 rng(0);
// mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

// ----------------------------------------------------------------------

void Main() {}

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
#ifdef LOCAL
  freopen("main.in", "r", stdin);
  // freopen("main.out", "w", stdout);
#endif
  Main();
  return 0;
}
