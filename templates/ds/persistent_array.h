#ifndef PERSISTENT_ARRAY_H_
#define PERSISTENT_ARRAY_H_

#include "segment_tree.h"

template <typename T>
struct PersistentArray {
  int n;
  int ts;
  PersistentArray(int n_) : n(n_), ts(0), seg(0, n_ - 1, true) {}
  void Set(int i, T val) {
    seg.ts = ts;
    seg.root = seg.Modify(i, i, [&](Info* info) { info->val = val; });
  }
  T Get(int i) {
    seg.ts = ts;
    return seg.GetAllNodes(i, i)[0]->val;
  }

 private:
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
