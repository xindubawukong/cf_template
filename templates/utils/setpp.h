#ifndef SETPP_H_
#define SETPP_H_

#include "ds/treap.h"

template <typename Key, typename Compare = std::less<Key>>
struct setpp {
  using value_type = Key;
  using size_type = unsigned int;

  setpp(const Compare& cmp_ = {}) : cmp(cmp_) {}
  setpp(std::initializer_list<value_type> init) : cmp({}) {
    for (auto x : init) insert(x);
  }
  setpp(const setpp& other) { *this = other; }
  setpp(setpp&& other) { *this = std::move(other); }

  ~setpp() { GC(treap.root); }

  setpp& operator=(const setpp& other) {
    clear();
    std::function<void(typename Treap<Info>::Node*)> Dfs =
        [&](typename Treap<Info>::Node* node) {
          if (!node) return;
          insert(node->val);
          Dfs(node->lch);
          Dfs(node->rch);
        };
    Dfs(other.treap.root);
    return *this;
  }
  setpp& operator=(setpp&& other) {
    clear();
    treap = other.treap;
    other.treap.root = nullptr;
    return *this;
  }

  size_type size() const { return treap.root ? treap.root->size : 0; }

  bool empty() const { return treap.root == nullptr; }

  void clear() {
    GC(treap.root);
    treap.root = nullptr;
  }

  bool contains(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    bool res = t2;
    treap.root = treap.Join(t1, t2, t3);
    return res;
  }

  Key get_kth(int k) {
    assert(k > 0 && k <= (int)size());
    auto [t1, t2, t3] = treap.SplitKth(treap.root, k);
    Key x = t2->val;
    treap.root = treap.Join(t1, t2, t3);
    return x;
  }

  int get_rank(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    int less = t1 ? t1->size : 0;
    treap.root = treap.Join(t1, t2, t3);
    return less + 1;
  }

  struct iterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Key;
    using difference_type = int;

    setpp* s;
    int k;

    iterator(setpp* s_, int k_) : s(s_), k(k_) {}
    iterator& operator+=(int d) {
      k += d;
      return *this;
    }
    iterator& operator-=(int d) { return (*this) += -d; }
    iterator& operator++() { return (*this) += 1; }
    iterator& operator--() { return (*this) -= 1; }
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
    Key operator*() const { return s->get_kth(k); }
    bool operator==(const iterator& other) const { return k == other.k; }
    operator std::string() { return "iterator(" + std::to_string(k) + ")"; }
  };
  using reverse_iterator = std::reverse_iterator<iterator>;

  iterator begin() { return iterator(this, 1); }
  iterator end() { return iterator(this, size() + 1); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }

  iterator lower_bound(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    int less = t1 ? t1->size : 0;
    treap.root = treap.Join(t1, t2, t3);
    return iterator(this, less + 1);
  }

  iterator upper_bound(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    int less = t1 ? t1->size : 0;
    if (t2) less++;
    treap.root = treap.Join(t1, t2, t3);
    return iterator(this, less + 1);
  }

  std::pair<iterator, bool> insert(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    bool res = false;
    if (!t2) {
      t2 = treap.Create();
      t2->val = x;
      res = true;
    }
    int k = (t1 ? t1->size : 0) + 1;
    treap.root = treap.Join(t1, t2, t3);
    return {iterator(this, k), res};
  }

  iterator erase(iterator pos) {
    auto [t1, t2, t3] = treap.SplitKth(treap.root, pos.k);
    int less = t1 ? t1->size : 0;
    GC(t2);
    treap.root = treap.Join(t1, t3);
    return iterator(this, less + 1);
  }

  size_type erase(const Key& x) {
    auto [t1, t2, t3] = Split(x);
    int res = t2 ? 1 : 0;
    if (t2) GC(t2);
    treap.root = treap.Join(t1, t3);
    return res;
  }

  operator std::string() {
    std::string s = "setpp[";
    for (auto it = begin(); it != end(); it++) {
      if constexpr (std::is_convertible_v<Key, std::string>) {
        s += static_cast<std::string>(*it);
      } else {
        s += std::to_string(*it);
      }
      if (it != prev(end())) s += ", ";
    }
    s += "]";
    return s;
  }

 private:
  struct Info {
    Key val;
    size_t size;
    auto Node() { return reinterpret_cast<typename Treap<Info>::Node*>(this); }
    Info() : size(1) {}
    bool NeedPushDown() { return false; }
    void PushDown() {}
    void Update() {
      auto lch = Node()->lch, rch = Node()->rch;
      size = 1;
      if (lch) size += lch->size;
      if (rch) size += rch->size;
    }
  };
  Treap<Info> treap;
  Compare cmp;

  auto Split(const Key& x) {
    return treap.Split(treap.root, [&](Info* info) {
      bool a = cmp(x, info->val), b = cmp(info->val, x);
      if (a) return -1;
      if (b) return 1;
      return 0;
    });
  }

  static void GC(typename Treap<Info>::Node* node) {
    if (!node) return;
    GC(node->lch);
    GC(node->rch);
    delete node;
  }
};

#endif  // SETPP_H_
