#ifndef BST_UTILS_H_
#define BST_UTILS_H_

#include <functional>
#include <vector>

namespace bst {

template <typename Tree, typename F>
void BuildTree(Tree& tree, int l, int r, F f) {
  std::function<typename Tree::Node*(int, int)> Build = [&](int l, int r) ->
      typename Tree::Node* {
        if (l > r) return nullptr;
        int mid = (l + r) / 2;
        typename Tree::info_t info;
        f(mid, &info);
        auto x = new typename Tree::Node(info);
        x->lch = Build(l, mid - 1);
        x->rch = Build(mid + 1, r);
        return tree.Update(x);
      };
  tree.root = Build(l, r);
}

template <typename Tree, typename Cmp>
typename Tree::Node* Search(Tree& tree, Cmp cmp) {
  auto x = tree.root;
  while (x) {
    tree.PushDown(x);
    int t = cmp(x);
    if (t < 0) {
      x = x->lch;
    } else if (t == 0) {
      return x;
    } else {
      x = x->rch;
    }
  }
  return nullptr;
}

// return {0, path} if found
// return {-1/1, path} if not found
template <typename Tree, typename Cmp>
std::pair<int, std::vector<typename Tree::Node*>> SearchPath(Tree& tree,
                                                             Cmp cmp) {
  auto x = tree.root;
  std::vector<typename Tree::Node*> res;
  int dir = -1;
  while (x) {
    tree.PushDown(x);
    res.push_back(x);
    int t = cmp(x);
    if (t < 0) {
      x = x->lch;
      dir = -1;
    } else if (t == 0) {
      return {0, res};
    } else {
      x = x->rch;
      dir = 1;
    }
  }
  return {dir, res};
}

template <typename Info>
struct KthCmp {
  KthCmp(int k_) : k(k_) {}
  int k;
  auto operator()() {
    return [&](Info* info) {
      auto x = info->Node();
      int left = x->lch ? x->lch->size : 0;
      if (k <= left) return -1;
      if (k == left + 1) return 0;
      k -= left + 1;
      return 1;
    };
  }
};

template <typename Tree>
typename Tree::Node* SearchKth(Tree& tree, int k) {
  return Search(tree, KthCmp<typename Tree::info_t>(k)());
}

template <typename Tree, typename Cmp>
int GetRank(Tree& tree, Cmp cmp) {
  int less = 0;
  auto rank_cmp = [&](typename Tree::Node* x) {
    int left = x->lch ? x->lch->size : 0;
    int t = cmp(x);
    if (t == 0) {
      less += left;
    } else if (t > 0) {
      less += left + 1;
    }
    return t;
  };
  Search(tree, rank_cmp);
  return less + 1;
}

template <typename Tree, typename F>
void Tranverse(Tree& tree, F f) {
  std::function<void(typename Tree::Node*)> Go = [&](typename Tree::Node* x) {
    if (!x) return;
    tree.PushDown(x);
    Go(x->lch);
    f(x);
    Go(x->rch);
  };
  Go(tree.root);
}

}  // namespace bst

#endif  // BST_UTILS_H_
