#ifndef SEGMENT_TREE_H_
#define SEGMENT_TREE_H_

#include <vector>

template <typename Info>
struct SegmentTree {
  struct Node {
    Node *lch, *rch;
    Info info;

    Node() : lch(nullptr), rch(nullptr) { info.node = this; }
  };

  int l_range, r_range;
  Node* root;

  SegmentTree(int l, int r) : l_range(l), r_range(r), root(nullptr) {}

  template <typename F>
  Node* Modify(Node* x, int l, int r, int from, int to, F f) {
    if (!x) {
      x = new Node();
    }
    if (from <= l && r <= to) {
      f(x->info, l, r);
      return x;
    }
    x->info.PushDown(l, r);
    int m = l + (r - l) / 2;
    if (from <= m) {
      x->lch = Modify(x->lch, l, m, from, to, f);
    }
    if (to > m) {
      x->rch = Modify(x->rch, m + 1, r, from, to, f);
    }
    x->info.Update();
    return x;
  }

  template <typename F>
  void Modify(int from, int to, F f) {
    root = Modify(root, l_range, r_range, from, to, f);
  }

  void GetAll(Node* x, int l, int r, int from, int to, std::vector<Info>& all) {
    if (!x) return;
    if (from <= l && r <= to) {
      all.push_back(x->info);
      return;
    }
    x->info.PushDown(l, r);
    int m = l + (r - l) / 2;
    if (from <= m) {
      GetAll(x->lch, l, m, from, to, all);
    }
    if (to > m) {
      GetAll(x->rch, m + 1, r, from, to, all);
    }
  }

  std::vector<Info> GetAll(int from, int to) {
    std::vector<Info> all;
    GetAll(root, l_range, r_range, from, to, all);
    return all;
  }
};

/*

struct Info {
  int64 sum, tag;
  SegmentTree<Info>::Node* node;

  Info(int64 sum = 0, int64 tag = 0) : sum(sum), tag(tag) {}

  void Add(int64 val, int l, int r) {
    tag += val;
    sum += val * (r - l + 1);
  }

  void PushDown(int l, int r) {
    if (tag == 0) return;
    int m = l + (r - l) / 2;
    if (node->lch) {
      node->lch->info.Add(tag, l, m);
    }
    if (node->rch) {
      node->rch->info.Add(tag, m + 1, r);
    }
    tag = 0;
  }

  void Update() {
    sum = 0;
    if (node->lch) {
      sum += node->lch->info.sum;
    }
    if (node->rch) {
      sum += node->rch->info.sum;
    }
  }
};

*/

#endif  // SEGMENT_TREE_H_
