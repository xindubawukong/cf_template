#ifndef PERSISTENT_ARRAY_H_
#define PERSISTENT_ARRAY_H_

#include <string>

#include "segment_tree.h"

template <typename T>
struct PersistentArray {
  struct Iterator {
    PersistentArray<T>* pa;
    int i;
    void operator=(T x) { pa->Set(i, x); }
    operator T() { return pa->Get(i); }
    operator std::string() {
      auto x = T(*this);
      if constexpr (std::is_integral<T>::value ||
                    std::is_floating_point<T>::value) {
        return std::to_string(x);
      } else {
        return static_cast<std::string>(x);
      }
    }
  };
  int n;
  int ts;
  PersistentArray(int n_) : n(n_), ts(0), seg(0, n_ - 1, true) {}
  Iterator operator[](int i) {
    Iterator it;
    it.pa = this;
    it.i = i;
    return it;
  }
  operator std::string() {
    std::string s = "PersistentArray[";
    for (int i = 0; i < n; i++) {
      s += std::string((*this)[i]);
      if (i < n - 1) s += ", ";
    }
    s += "]";
    return s;
  }

 private:
  void Set(int i, T val) {
    seg.ts = ts;
    seg.root = seg.Modify(i, i, [&](Info* info) { info->val = val; });
  }
  T Get(int i) {
    seg.ts = ts;
    return seg.GetAllNodes(i, i)[0]->val;
  }
  struct Info {
    typename SegmentTree<Info>::Node* Node() {
      return reinterpret_cast<typename SegmentTree<Info>::Node*>(this);
    }
    T val;
    Info() {}
    bool NeedPushDown() { return false; }
    void PushDown() {}
    void Update() {}
  };
  SegmentTree<Info> seg;
};

#endif  // PERSISTENT_ARRAY_H_
