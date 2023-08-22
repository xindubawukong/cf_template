#ifndef DELAYED_SEQUENCE_H_
#define DELAYED_SEQUENCE_H_

#include <cassert>
#include <type_traits>

template <typename F>
struct DelayedSequence {
  using T = std::invoke_result_t<F, size_t>;
  using value_type = T;

  size_t first, last;
  F f;

  DelayedSequence(size_t n, F _f) : first(0), last(n), f(_f) {}
  DelayedSequence(size_t first_, size_t last_, F f_)
      : first(first_), last(last_), f(f_) {}

  struct iterator {
    // TBD
  };

  iterator begin() {}
  iterator end() {}

  T operator[](size_t i) const { return f(i); }
  size_t size() const {
    assert(first <= last);
    return last - first;
  }
  bool empty() const { return size() == 0; }
};

#endif  // DELAYED_SEQUENCE_H_
