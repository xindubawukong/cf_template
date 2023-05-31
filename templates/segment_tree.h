#ifndef SEGMENT_TREE_H_
#define SEGMENT_TREE_H_

#include <cassert>
#include <functional>
#include <vector>

/*
struct Info {
  SegmentTree<Info>::Node* Node() {
    return reinterpret_cast<SegmentTree<Info>::Node*>(this);
  }
  Info() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {}
};
*/

template <typename Info>
struct SegmentTree {
  struct Node {
    Info info;
    int l, r, ts;
    Node *lch, *rch;
    Node(int l_, int r_, int ts_ = 0)
        : l(l_), r(r_), ts(ts_), lch(nullptr), rch(nullptr) {}
    Node(Node *x, int ts_)
        : l(x->l), r(x->r), ts(ts_), lch(x->lch), rch(x->rch), info(x->info) {}
  };

  int l_range, r_range;
  Node *root;
  bool persist;
  int ts;  // plus ts if persistence is wanted
  SegmentTree(int l, int r, bool persist_ = false)
      : l_range(l), r_range(r), root(nullptr), persist(persist_), ts(0) {
    assert(l_range <= r_range);
  }

  void PushDown(Node *x) {
    if (x->info.NeedPushDown()) {
      if (persist) {
        if (x->lch && x->lch->ts != ts) x->lch = new Node(x->lch, ts);
        if (x->rch && x->rch->ts != ts) x->rch = new Node(x->rch, ts);
      }
      x->info.PushDown();
    }
  }

  template <typename F>
  Node *Modify(int from, int to, F f) {
    assert(l_range <= from && from <= to && to <= r_range);
    root = Modify(root, l_range, r_range, from, to, f);
    return root;
  }

  std::vector<Node *> GetAllNodes(int from, int to) {
    assert(l_range <= from && from <= to && to <= r_range);
    std::vector<Node *> all;
    std::function<void(Node *, int, int, int, int)> GetAllNodes =
        [&](Node *x, int l, int r, int from, int to) {
          if (!x) return;
          if (from <= l && r <= to) {
            all.push_back(x);
            return;
          }
          PushDown(x);
          int m = l + (r - l) / 2;
          if (from <= m) {
            GetAllNodes(x->lch, l, m, from, to);
          }
          if (to > m) {
            GetAllNodes(x->rch, m + 1, r, from, to);
          }
        };
    GetAllNodes(root, l_range, r_range, from, to);
    return all;
  }

  template <typename F>
  int GetFirstAfter(int p, F f) {
    assert(l_range <= p && p <= r_range);
    auto nodes = GetAllNodes(p, r_range);
    for (auto x : nodes) {
      if (f(x->info)) {
        while (x->r > x->l) {
          assert(f(x->info));
          PushDown(x);
          if (x->lch && f(x->lch->info)) {
            x = x->lch;
          } else {
            x = x->rch;
          }
        }
        return x->l;
      }
    }
    return r_range + 1;
  }

  template <typename F>
  int GetLastBefore(int p, F f) {
    assert(l_range <= p && p <= r_range);
    auto nodes = GetAllNodes(l_range, p);
    reverse(nodes.begin(), nodes.end());
    for (auto x : nodes) {
      if (f(x->info)) {
        while (x->r > x->l) {
          assert(f(x->info));
          PushDown(x);
          if (x->rch && f(x->rch->info)) {
            x = x->rch;
          } else {
            x = x->lch;
          }
        }
        return x->l;
      }
    }
    return l_range - 1;
  }

  template <typename F>
  void TranverseLeaf(Node *x, F f) {
    if (!x) return;
    if (x->l == x->r) {
      f(x->info);
      return;
    }
    TranverseLeaf(x->lch, f);
    TranverseLeaf(x->rch, f);
  };

  template <typename F>
  static Node *BuildTree(int l, int r, F f) {
    Node *x = new Node(l, r);
    if (l == r) {
      f(l, x->info);
      return x;
    }
    int m = l + (r - l) / 2;
    x->lch = BuildTree(l, m, f);
    x->rch = BuildTree(m + 1, r, f);
    x->info.Update();
    return x;
  }

 private:
  template <typename F>
  Node *Modify(Node *x, int l, int r, int from, int to, F f) {
    if (!x) {
      x = new Node(l, r, ts);
    } else if (persist && x->ts != ts) {
      x = new Node(x, ts);
    }
    if (from <= l && r <= to) {
      f(x->info);
      return x;
    }
    PushDown(x);
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
};

#endif  // SEGMENT_TREE_H_
