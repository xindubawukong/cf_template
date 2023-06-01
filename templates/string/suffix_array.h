#ifndef SUFFIX_ARRAY_H_
#define SUFFIX_ARRAY_H_

#include <algorithm>
#include <cassert>
#include <iterator>
#include <tuple>
#include <vector>

#include "parlay/parallel.h"
#include "parlay/primitives.h"
#include "parlay/sequence.h"
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
  RadixSort(sa, [&](int i) { return a[i]; });
  for (int i = 0; i < n; i++) sum[i] = 0;
  for (int i = 0; i < n; i++) {
    if (i != 0 && a[sa[i]] != a[sa[i - 1]]) sum[i]++;
  }
  for (int i = 1; i < n; i++) sum[i] += sum[i - 1];
  for (int i = 0; i < n; i++) rank[sa[i]] = sum[i];
  for (int len = 1; len <= n; len *= 2) {
    auto f = [&](int i) { return i + len < n ? rank[i + len] + 1 : 0; };
    RadixSort(sa, [&](int i) { return f(i); });
    RadixSort(sa, [&](int i) { return rank[i]; });
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

template <typename Seq>
parlay::sequence<unsigned int> SuffixArrayDC3(const Seq& s0) {
  int n = s0.size();
  auto a = parlay::sequence<unsigned int>(n);
  for (int i = 0; i < n; i++) a[i] = s0[i];
  SAPreProcess(a);

  std::function<parlay::sequence<unsigned int>(
      const parlay::sequence<unsigned int>&)>
      DC3Internal = [&](const parlay::sequence<unsigned int>& s) {
        debug("DC3Internal", s.size());
        // 1 <= s[i] <= n
        int n = s.size();
        assert(*std::min_element(s.begin(), s.end()) > 0);
        assert(*std::max_element(s.begin(), s.end()) <= n);
        if (n <= 10) {
          auto sa_ = get<0>(SuffixArrayPrefixDoubling(s));
          auto sa = parlay::sequence<unsigned int>(n);
          for (int i = 0; i < n; i++) sa[i] = sa_[i];
          return sa;
        }
        int m = n;
        while (m % 3) m++;
        parlay::sequence<unsigned int> ss(m + 3);
        for (int i = 0; i < m + 3; i++) ss[i] = i < n ? s[i] : 0;
        auto a12 = parlay::sequence<unsigned int>(m / 3 * 2, 0);
        for (int i = 0; i < m; i++) {
          if (i % 3) a12[(i / 3) * 2 + i % 3 - 1] = i;
        };
        RadixSort(a12, [&](auto i) { return ss[i + 2]; });
        RadixSort(a12, [&](auto i) { return ss[i + 1]; });
        RadixSort(a12, [&](auto i) { return ss[i]; });
        parlay::sequence<unsigned int> same(a12.size());
        for (int i = 0; i < a12.size(); i++) {
          if (i == 0) same[i] = 1;
          auto p = a12[i], q = a12[i - 1];
          same[i] = ss[p] != ss[q] || ss[p + 1] != ss[q + 1] ||
                    ss[p + 2] != ss[q + 2];
        }
        for (int i = 1; i < same.size(); i++) same[i] += same[i - 1];
        auto sum = same.back();
        // if can not sort by 3 characters, call DC3 recursively
        if (sum < m / 3 * 2) {
          auto tao = parlay::sequence<unsigned int>(m);
          for (int i = 0; i < a12.size(); i++) tao[a12[i]] = same[i];
          for (int i = 0; i < m / 3; i++) {
            a12[i] = tao[i * 3 + 1];
            a12[i + m / 3] = tao[i * 3 + 2];
          }
          a12 = DC3Internal(a12);
          for (int i = 0; i < a12.size(); i++) {
            if (a12[i] < m / 3) {
              a12[i] = a12[i] * 3 + 1;
            } else {
              a12[i] = (a12[i] - m / 3) * 3 + 2;
            }
          }
        }
        auto rank = parlay::sequence<unsigned int>(m);
        for (int i = 0; i < a12.size(); i++) rank[a12[i]] = i;
        auto a0 = parlay::sequence<unsigned int>(m / 3);
        for (int i = 0; i < m / 3; i++) a0[i] = i * 3;
        RadixSort(a0, [&](auto i) { return rank[i + 1]; });
        RadixSort(a0, [&](auto i) { return ss[i]; });
        parlay::sequence<unsigned int> a;
        auto cmp = [&](auto i, auto j) {
          assert(i % 3 == 0 && j % 3 > 0);  // i from a0 and j from a12
          if (j % 3 == 1) {
            if (ss[i] != ss[j]) return ss[i] < ss[j];
            return rank[i + 1] < rank[j + 1];
          } else {
            if (ss[i] != ss[j]) return ss[i] < ss[j];
            if (ss[i + 1] != ss[j + 1]) return ss[i + 1] < ss[j + 1];
            return rank[i + 2] < rank[j + 2];
          }
        };
        auto Push = [&](int x) {
          if (x < n) a.push_back(x);
        };
        for (int i = 0, j = 0; i < a0.size() || j < a12.size();) {
          if (j == a12.size() || (i < a0.size() && cmp(a0[i], a12[j]))) {
            Push(a0[i++]);
          } else {
            Push(a12[j++]);
          }
        }
        return a;
      };
  return DC3Internal(a);
}

#endif  // SUFFIX_ARRAY_H_
