#ifndef RADIX_SORT_H_
#define RADIX_SORT_H_

#include <algorithm>
#include <cassert>
#include <vector>

// mm = max{f(a[i])}
template <typename Seq, typename F>
void RadixSort(Seq& a, F f) {
  int n = a.size();
  int mm = -1;
  std::vector<int> sum, b, at(n);
  for (int i = 0; i < n; i++) {
    int t = f(a[i]);
    assert(t >= 0);
    if (t > mm) {
      mm = t;
      sum.resize(mm + 2);
      b.resize(mm + 1);
    }
    at[i] = b[t]++;
  }
  for (int i = 0; i <= mm; i++) sum[i + 1] = sum[i] + b[i];
  auto a1 = a;
  for (int i = 0; i < n; i++) {
    a1[sum[f(a[i])] + at[i]] = a[i];
  }
  a = std::move(a1);
}

#endif  // RADIX_SORT_H_
