#ifndef TREAP_H_
#define TREAP_H_

#include <optional>
#include <random>
#include <vector>

#include "debug.h"

namespace treap {

template <typename T>
struct Treap {
  struct Node {
    unsigned int priority;
    Node *lch, *rch;
    T entry;

    Node(T entry) : lch(nullptr), rch(nullptr), entry(entry) {
      static std::mt19937 rng(0);
      priority = rng();
    }

    Node* Update() {
      entry.Update(lch ? &lch->entry : nullptr, rch ? &rch->entry : nullptr);
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

  std::tuple<Node*, Node*, Node*> Split(Node* x, typename T::key_t key) {
    if (x == nullptr) {
      return {nullptr, nullptr, nullptr};
    }
    if (T::Less(key, x->entry.GetKey())) {
      auto [w, y, z] = Split(x->lch, key);
      x->lch = nullptr;
      return {w, y, Merge(z, x->Update())};
    } else if (T::Less(x->entry.GetKey(), key)) {
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

  void Insert(T entry) {
    auto [x, y, z] = Split(root, entry.GetKey());
    if (!y) {
      y = new Node(entry);
    }
    root = Merge(x, y, z);
  }

  void Delete(typename T::key_t key) {
    auto [x, y, z] = Split(root, key);
    if (y) delete y;
    root = Merge(x, z);
  }

  int GetRank(typename T::key_t key) {
    auto x = root;
    int less = 0;
    while (x) {
      if (T::Less(key, x->entry.GetKey())) {
        x = x->lch;
      } else {
        less += x->lch ? x->lch->entry.size : 0;
        if (T::Less(x->entry.GetKey(), key)) {
          less++;
          x = x->rch;
        } else {
          break;
        }
      }
    }
    return less + 1;
  }

  std::optional<T> GetKth(int k) {
    if (!root || root->entry.size < k) return {};
    auto x = root;
    while (x) {
      int left = x->lch ? x->lch->entry.size : 0;
      if (k <= left) {
        x = x->lch;
      } else {
        k -= left;
        if (k == 1) return x->entry;
        k--;
        x = x->rch;
      }
    }
    assert(false);
  }

  std::optional<T> GetPrev(typename T::key_t key) {
    auto [x, y, z] = Split(root, key);
    auto t = x;
    std::optional<T> res = {};
    if (t) {
      while (t->rch) t = t->rch;
      res = t->entry;
    }
    root = Merge(x, y, z);
    return res;
  }

  std::optional<T> GetNext(typename T::key_t key) {
    auto [x, y, z] = Split(root, key);
    auto t = z;
    std::optional<T> res = {};
    if (t) {
      while (t->lch) t = t->lch;
      res = t->entry;
    }
    root = Merge(x, y, z);
    return res;
  }
};

}  // namespace treap

// struct Entry {
//   using key_t = int;
//   int val;
//   int size;

//   Entry(int val) : val(val), size(1) {}

//   key_t GetKey() { return val; }

//   void Update(Entry* l, Entry* r) {
//     size = 1;
//     if (l) size += l->size;
//     if (r) size += r->size;
//   }

//   static bool Less(const key_t& a, const key_t& b) { return a < b; }
// };

#endif  // TREAP_H_