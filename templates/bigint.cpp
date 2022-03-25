#ifdef __clang__
#define LOCAL
#endif

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using float64 = long double;
using int64 = long long;
using int128 = __int128_t;
using uint64 = unsigned long long;

// --------------------------- xindubawukong ---------------------------

struct BigInt {
  vector<int> a;
  void Update() {
    while (a.back() == 0 && a.size() > 1) a.pop_back();
    for (int i = 0; i < a.size(); i++) {
      if (a[i] > 9) {
        if (i + 1 >= a.size()) {
          a.push_back(0);
        }
        a[i + 1] += a[i] / 10;
        a[i] %= 10;
      }
    }
  }

  BigInt(int x) {
    a.resize(1);
    a[0] = x;
    Update();
  }

  void operator*=(int x) {
    for (int i = 0; i < a.size(); i++) {
      a[i] *= x;
    }
    Update();
  }

  void Output() {
    for (int i = a.size() - 1; i >= 0; i--) {
      printf("%d", a[i]);
    }
    printf("\n");
  }
};

BigInt operator+(const BigInt& a, const BigInt& b) {
  BigInt c(0);
  int na = a.a.size();
  int nb = b.a.size();
  c.a.resize(max(na, nb));
  for (int i = 0; i < max(na, nb); i++) {
    int x = i < na ? a.a[i] : 0;
    int y = i < nb ? b.a[i] : 0;
    c.a[i] = x + y;
  }
  c.Update();
  return c;
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
