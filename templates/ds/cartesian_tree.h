#ifndef CARTESIAN_TREE_H_
#define CARTESIAN_TREE_H_

#include <vector>

template <typename Seq>
struct CartesianTree {
  int n, root;
  std::vector<int> lch, rch, fa;
  CartesianTree(const Seq& a) {
    n = a.size();
    root = -1;
    lch.assign(n, -1), rch.assign(n, -1), fa.assign(n, -1);
    std::vector<int> b(n);
    int num = 0;
    for (int i = 0; i < n; i++) {
      int k = num - 1;
      while (k >= 0 && a[b[k]] > a[i]) k--;
      if (k >= 0) rch[b[k]] = i;
      if (k < num - 1) lch[i] = b[k + 1];
      b[++k] = i;
      num = k + 1;
    }
    root = b[0];
    b = {root};
    for (int i = 0; i < (int)b.size(); i++) {
      int x = b[i];
      if (lch[x] != -1) {
        fa[lch[x]] = x;
        b.push_back(lch[x]);
      }
      if (rch[x] != -1) {
        fa[rch[x]] = x;
        b.push_back(rch[x]);
      }
    }
    assert((int)b.size() == n);
  }
};

#endif  // CARTESIAN_TREE_H_
