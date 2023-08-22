#ifndef RANGE_MIN_H_
#define RANGE_MIN_H_

#include <functional>
#include <vector>

#include "utils/delayed_sequence.h"
#include "utils/sparse_table.h"

template <typename Seq, typename Less = std::less<typename Seq::value_type>>
struct RMQ {
  Seq a;
  Less less;
  SparseTable<DelayedSequence<std::function<int(int)>>,
              std::function<int(int, int)>>
      st;

  RMQ(const Seq& a_, const Less& less_ = {})
      : a(a_),
        less(less_),
        st(DelayedSequence(a.size(), std::function<int(int)>(
                                         [](int i) -> int { return i; })),
           [&](int i, int j) { return less(a[i], a[j]) ? i : j; }) {}

  int QueryIndex(int l, int r) const { return st.Query(l, r); }
  auto QueryValue(int l, int r) const { return a[QueryIndex(l, r)]; }
};

#endif  // RANGE_MIN_H_
