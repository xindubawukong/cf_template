#ifndef TREAP_H_
#define TREAP_H_

#include <random>
#include <tuple>

template <typename Info>
struct Treap {
  struct Node {
    unsigned int priority;
    Node *lch, *rch;
    Info info;

    Node(Info info) : lch(nullptr), rch(nullptr), info(info) {
      info.node = this;
      static std::mt19937 rng(0);
      priority = rng();
    }

    Node* Update() {
      info.Update();
      return this;
    }
  };

  Node* Merge(Node* x, Node* y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;
    if (x->priority > y->priority) {
      x->rch = Merge(x->rch, y);
      return x->Update();
    } else {
      y->lch = Merge(x, y->lch);
      return y->Update();
    }
  }

  Node* Merge(Node* x, Node* y, Node* z) {
    if (x == nullptr) return Merge(y, z);
    if (y == nullptr) return Merge(x, z);
    if (z == nullptr) return Merge(x, y);
    if (y->priority > x->priority && y->priority > z->priority) {
      y->lch = x;
      y->rch = z;
      return y->Update();
    } else if (x->priority > z->priority) {
      x->rch = Merge(x->rch, y, z);
      return x->Update();
    } else {
      z->lch = Merge(x, y, z->lch);
      return z->Update();
    }
  }

  std::tuple<Node*, Node*, Node*> Split(Node* x, typename Info::key_t key) {
    if (x == nullptr) {
      return {nullptr, nullptr, nullptr};
    }
    if (Info::Less(key, x->info.GetKey())) {
      auto [w, y, z] = Split(x->lch, key);
      x->lch = nullptr;
      return {w, y, Merge(z, x->Update())};
    } else if (Info::Less(x->info.GetKey(), key)) {
      auto [w, y, z] = Split(x->rch, key);
      x->rch = nullptr;
      return {Merge(x->Update(), w), y, z};
    } else {
      auto l = x->lch, r = x->rch;
      x->lch = x->rch = nullptr;
      return {l, x->Update(), r};
    }
  }

  Node* root;
  Treap(Node* root = nullptr) : root(root) {}

  void Insert(Info info) {
    auto [x, y, z] = Split(root, info.GetKey());
    if (!y) {
      y = new Node(info);
    }
    root = Merge(x, y, z);
  }

  void Delete(typename Info::key_t key) {
    auto [x, y, z] = Split(root, key);
    if (y) delete y;
    root = Merge(x, z);
  }
};

/*

struct Info {
  using key_t = int;
  Treap<Info>::Node* node;
  int val;
  int size;

  Info(int val) : val(val), size(1) {}

  key_t GetKey() { return val; }

  void Update() {
    size = 1;
    if (node->lch) size += node->lch->info.size;
    if (node->rch) size += node->rch->info.size;
  }

  static bool Less(const key_t& a, const key_t& b) { return a < b; }
};

*/

#endif  // TREAP_H_