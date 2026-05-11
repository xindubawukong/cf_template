#ifndef DELAYED_SEQUENCE_H_
#define DELAYED_SEQUENCE_H_

#include <cassert>
#include <cstddef>
#include <iterator>
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
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::add_pointer_t<value_type>;
    using reference = T;

    const DelayedSequence* seq;
    size_t index;

    iterator() : seq(nullptr), index(0) {}
    iterator(const DelayedSequence* seq_, size_t index_)
        : seq(seq_), index(index_) {}

    reference operator*() const {
      assert(seq != nullptr);
      assert(seq->first <= index && index < seq->last);
      return seq->f(index);
    }
    reference operator[](difference_type d) const { return *(*this + d); }

    iterator& operator+=(difference_type d) {
      assert(seq != nullptr);
      if (d >= 0) {
        auto delta = static_cast<size_t>(d);
        assert(delta <= seq->last - index);
        index += delta;
      } else {
        auto delta = static_cast<size_t>(-d);
        assert(delta <= index - seq->first);
        index -= delta;
      }
      return *this;
    }
    iterator& operator-=(difference_type d) { return *this += -d; }

    iterator& operator++() { return *this += 1; }
    iterator& operator--() { return *this -= 1; }
    iterator operator++(int) {
      iterator t = *this;
      ++(*this);
      return t;
    }
    iterator operator--(int) {
      iterator t = *this;
      --(*this);
      return t;
    }

    iterator operator+(difference_type d) const {
      iterator t = *this;
      t += d;
      return t;
    }
    iterator operator-(difference_type d) const {
      iterator t = *this;
      t -= d;
      return t;
    }
    difference_type operator-(const iterator& other) const {
      assert(seq == other.seq);
      return static_cast<difference_type>(index) -
             static_cast<difference_type>(other.index);
    }

    bool operator==(const iterator& other) const {
      return seq == other.seq && index == other.index;
    }
    bool operator!=(const iterator& other) const { return !(*this == other); }
    bool operator<(const iterator& other) const {
      assert(seq == other.seq);
      return index < other.index;
    }
    bool operator>(const iterator& other) const { return other < *this; }
    bool operator<=(const iterator& other) const { return !(other < *this); }
    bool operator>=(const iterator& other) const { return !(*this < other); }
  };
  using reverse_iterator = std::reverse_iterator<iterator>;

  iterator begin() const { return iterator(this, first); }
  iterator end() const { return iterator(this, last); }
  iterator cbegin() const { return begin(); }
  iterator cend() const { return end(); }
  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() const { return reverse_iterator(begin()); }

  T operator[](size_t i) const { return f(i); }
  size_t size() const {
    assert(first <= last);
    return last - first;
  }
  bool empty() const { return size() == 0; }
};

#endif  // DELAYED_SEQUENCE_H_
