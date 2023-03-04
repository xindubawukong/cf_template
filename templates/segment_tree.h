#ifndef SEGMENT_TREE_H_
#define SEGMENT_TREE_H_

#include <vector>

template <typename Info>
struct SegmentTree {
  struct Node {
    int l, r;
    Node *lch, *rch;
    Info info;
    Node(int l_, int r_) : l(l_), r(r_), lch(nullptr), rch(nullptr) {
      info.node = this;
    }
    Node(Node* x) : l(x->l), r(x->r), lch(x->lch), rch(x->rch), info(x->info) {
      info.node = this;
    }
  };

  int l_range, r_range;
  Node* root;
  bool persist;
  SegmentTree(int l, int r, bool persist_ = false)
      : l_range(l), r_range(r), root(nullptr), persist(persist_) {}

  template <typename F>
  Node* Modify(int from, int to, F f) {
    root = Modify(root, l_range, r_range, from, to, f);
    return root;
  }

  std::vector<Node*> GetAll(int from, int to) {
    std::vector<Node*> all;
    GetAll(root, l_range, r_range, from, to, all);
    return all;
  }

  template <typename T, typename F>
  static Node* BuildTree(std::vector<T>& arr, int l, int r, F f) {
    Node* x = new Node(l, r);
    if (l == r) {
      f(x->info, arr[l]);
      return x;
    }
    int m = l + (r - l) / 2;
    x->lch = BuildTree(arr, l, m, f);
    x->rch = BuildTree(arr, m + 1, r, f);
    x->info.Update();
    return x;
  }

 private:
  template <typename F>
  Node* Modify(Node* x, int l, int r, int from, int to, F f) {
    if (!x) {
      x = new Node(l, r);
    } else if (persist && x == root) {
      x = new Node(x);
    }
    if (from <= l && r <= to) {
      f(x->info);
      return x;
    }
    if (persist) {
      if (x->lch) x->lch = new Node(x->lch);
      if (x->rch) x->rch = new Node(x->rch);
    }
    x->info.PushDown();
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

  void GetAll(Node* x, int l, int r, int from, int to,
              std::vector<Node*>& all) {
    if (!x) return;
    if (from <= l && r <= to) {
      all.push_back(x);
      return;
    }
    if (persist) {
      if (x->lch) x->lch = new Node(x->lch);
      if (x->rch) x->rch = new Node(x->rch);
    }
    x->info.PushDown();
    int m = l + (r - l) / 2;
    if (from <= m) {
      GetAll(x->lch, l, m, from, to, all);
    }
    if (to > m) {
      GetAll(x->rch, m + 1, r, from, to, all);
    }
  }
};

/*
struct Info {
  int size;
  SegmentTree<Info>::Node* node;
  Info() : size(0) {}
  void PushDown() {}
  void Update() {
    size = 0;
    if (node->lch) size += node->lch->info.size;
    if (node->rch) size += node->rch->info.size;
  }
};
*/

#endif  // SEGMENT_TREE_H_
