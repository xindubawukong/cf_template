#ifndef SUFFIX_ARRAY_H_
#define SUFFIX_ARRAY_H_

#include <vector>

#include "utils/radix_sort.h"

// Adjust a[i] to 1..n
template <typename Seq>
void SAPreProcess(Seq& a) {
  int n = a.size();
  int minx = *std::min_element(a.begin(), a.end());
  int maxx = *std::max_element(a.begin(), a.end());
  if (minx >= 1 && maxx <= n) return;
  if (minx == 0 && maxx < n) {
    for (int i = 0; i < n; i++) a[i]++;
    return;
  }
  auto b = a;
  sort(b.begin(), b.end());
  for (int i = 0; i < n; i++) {
    a[i] = std::lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;
  }
}

template <typename Seq>
auto SuffixArrayPrefixDoubling(const Seq& s0) {
  int n = s0.size();
  std::vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = s0[i];
  SAPreProcess(a);
  std::vector<int> sa(n), rank(n), lcp(n), sum(n);
  for (int i = 0; i < n; i++) sa[i] = i;
  RadixSort(sa, n, [&](int i) { return a[i]; });
  for (int i = 0; i < n; i++) sum[i] = 0;
  for (int i = 0; i < n; i++) {
    if (i != 0 && a[sa[i]] != a[sa[i - 1]]) sum[i]++;
  }
  for (int i = 1; i < n; i++) sum[i] += sum[i - 1];
  for (int i = 0; i < n; i++) rank[sa[i]] = sum[i];
  for (int len = 1; len <= n; len *= 2) {
    auto f = [&](int i) { return i + len < n ? rank[i + len] + 1 : 0; };
    RadixSort(sa, n, [&](int i) { return f(i); });
    RadixSort(sa, n, [&](int i) { return rank[i]; });
    for (int i = 0; i < n; i++) sum[i] = 0;
    for (int i = 0; i < n; i++) {
      if (i != 0 &&
          (f(sa[i]) != f(sa[i - 1]) || rank[sa[i]] != rank[sa[i - 1]]))
        sum[i]++;
    }
    for (int i = 1; i < n; i++) sum[i] += sum[i - 1];
    for (int i = 0; i < n; i++) rank[sa[i]] = sum[i];
  }
  for (int i = 0, p = 0; i < n; i++) {
    if (rank[i] == 0) continue;
    int j = sa[rank[i] - 1];
    while (a[i + p] == a[j + p]) p++;
    lcp[rank[i]] = p;
    if (p) p--;
  }
  return std::make_tuple(sa, rank, lcp);
}

#endif  // SUFFIX_ARRAY_H_
