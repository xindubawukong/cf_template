#ifndef TREAP_H_
#define TREAP_H_

#include <random>
#include <tuple>

/*
struct Info {
  Treap<Info>::Node* node;
  int val, size;
  Info() {}
  bool NeedPushDown() { return rev; }
  void PushDown() {}
  void Update() {
    size = 1;
    if (node->lch) {
      size += node->lch->info.size;
    }
    if (node->rch) {
      size += node->rch->info.size;
    }
  }
};
*/

template <typename Info>
struct Treap {
  struct Node {
    unsigned int priority;
    int ts;
    Node *lch, *rch;
    Info info;
    Node(Info info_, int ts_ = 0)
        : ts(ts_), lch(nullptr), rch(nullptr), info(info_) {
      info.node = this;
      static std::mt19937 rng(0);
      priority = rng();
    }
    // when copying a node, must be in persist mode
    Node(Node* x, int ts_)
        : priority(x->priority),
          ts(ts_),
          lch(x->lch),
          rch(x->rch),
          info(x->info) {
      info.node = this;
    }
  };

  Node* root;
  bool persist;
  int ts;  // plus version if persistence is wanted
  Treap(bool persist_ = false) : root(nullptr), persist(persist_), ts(0) {}

  Node* Update(Node* x) {
    x->info.Update();
    return x;
  }

  void PushDown(Node* x) {
    if (x->info.NeedPushDown()) {
      if (persist) {
        if (x->lch && x->lch->ts != ts) x->lch = new Node(x->lch, ts);
        if (x->rch && x->rch->ts != ts) x->rch = new Node(x->rch, ts);
      }
      x->info.PushDown();
    }
  }

  Node* Join(Node* x, Node* y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
      if (y->ts != ts) y = new Node(y, ts);
    }
    PushDown(x);
    PushDown(y);
    if (x->priority > y->priority) {
      x->rch = Join(x->rch, y);
      return Update(x);
    } else {
      y->lch = Join(x, y->lch);
      return Update(y);
    }
  }

  Node* Join(Node* x, Node* y, Node* z) {
    if (x == nullptr) return Join(y, z);
    if (y == nullptr) return Join(x, z);
    if (z == nullptr) return Join(x, y);
    assert(y->lch == nullptr && y->rch == nullptr);  // y must be single node
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
      if (y->ts != ts) y = new Node(y, ts);
      if (z->ts != ts) z = new Node(z, ts);
    }
    PushDown(x);
    PushDown(z);
    if (x->priority > y->priority && x->priority > z->priority) {
      x->rch = Join(x->rch, y, z);
      return Update(x);
    } else if (y->priority > z->priority) {
      y->lch = x;
      y->rch = z;
      return Update(y);
    } else {
      z->lch = Join(x, y, z->lch);
      return Update(z);
    }
  }

  template <typename Cmp>
  std::tuple<Node*, Node*, Node*> Split(Node* x, Cmp cmp) {
    if (x == nullptr) {
      return {nullptr, nullptr, nullptr};
    }
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
    }
    PushDown(x);
    int d = cmp(x->info);
    if (d == 0) {
      auto l = x->lch, r = x->rch;
      if (l && l->ts != ts) l = new Node(l, ts);
      if (r && r->ts != ts) r = new Node(r, ts);
      x->lch = x->rch = nullptr;
      return {l, Update(x), r};
    } else if (d < 0) {
      auto [w, y, z] = Split(x->lch, cmp);
      x->lch = nullptr;
      return {w, y, Join(z, Update(x))};
    } else {
      auto [w, y, z] = Split(x->rch, cmp);
      x->rch = nullptr;
      return {Join(Update(x), w), y, z};
    }
  }

  auto SplitKth(Node* x, int k) {
    return Split(x, [&k](Info& info) {
      int left = info.node->lch ? info.node->lch->info.size : 0;
      if (k <= left) return -1;
      if (k == left + 1) return 0;
      k -= left + 1;
      return 1;
    });
  }

  template <typename F>
  void Tranverse(F f) {
    std::function<void(Node*)> Go = [&](Node* x) {
      if (!x) return;
      PushDown(x);
      Go(x->lch);
      f(x->info);
      Go(x->rch);
    };
    Go(root);
  }
};

#endif  // TREAP_H_