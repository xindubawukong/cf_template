#ifndef ARRAY_H_
#define ARRAY_H_

#include <vector>

template <typename T = int>
struct Array {
  Array(T l, T r) {
    assert(l <= r);
    this->l = l, this->r = r;
    data.resize(r - l + 1);
  }

  T& operator[](T i) {
    assert(l <= i && i <= r);
    return data[i];
  }

 private:
  T l, r;
  std::vector<int> data;
};

#endif  // ARRAY_H_
