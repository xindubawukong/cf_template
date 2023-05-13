#ifndef RADIX_SORT_H_
#define RADIX_SORT_H_

#include <algorithm>
#include <cassert>
#include <vector>

// mm = max{f(a[i])}
template <typename Seq, typename F>
void RadixSort(Seq& a, int mm, F f) {
  int n = a.size();
  std::vector<int> sum(mm + 2), b(mm + 1), at(n);
  for (int i = 0; i < n; i++) {
    int t = f(a[i]);
    assert(t <= mm);
    at[i] = b[t]++;
  }
  for (int i = 0; i <= mm; i++) sum[i + 1] = sum[i] + b[i];
  auto a1 = a;
  for (int i = 0; i < n; i++) {
    a1[sum[f(a[i])] + at[i]] = a[i];
  }
  a = a1;
}

#endif  // RADIX_SORT_H_
