#ifndef SPLAY_H_
#define SPLAY_H_

template <typename Info>
struct SplayTree {
  struct Node {
    Info info;
    Node *lch, *rch, *fa;
    Node(Info info_) : info(info_){};
  };

  Node* root;
  SplayTree() : root(nullptr) {}

  void Zig(Node* t) {
    auto y = t->fa;
    y->info.PushDown();
    t->info.PushDown();
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) {
        y->fa->lch = t;
      } else {
        y->fa->rch = t;
      }
    }
    y->lch = t->rch;
    if (t->rch) t->rch->fa = y;
    t->rch = y;
    y->fa = t;
    y->info.Update();
    t->info.Update();
  }

  void Zag(Node* t) {
    auto y = t->fa;
    y->info.PushDown();
    t->info.PushDown();
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) {
        y->fa->lch = t;
      } else {
        y->fa->rch = t;
      }
    }
    y->rch = t->lch;
    if (t->lch) t->lch->fa = y;
    t->lch = y;
    y->fa = t;
    y->info.Update();
    t->info.Update();
  }

  // splay t under p
  void Splay(Node* t, Node* p = nullptr) {
    assert(t);
    while (t->fa != p) {
      auto y = t->fa;
      if (y->fa == p) {
        if (t == y->lch)
          Zig(t);
        else
          Zag(t);
      } else {
        if (y == y->fa->lch) {
          if (t == y->lch)
            Zig(y), Zig(t);
          else
            Zag(t), Zig(t);
        } else {
          if (t == y->rch)
            Zag(y), Zag(t);
          else
            Zig(t), Zag(t);
        }
      }
    }
    if (!p) root = t;
  }
};

#endif  // SPLAY_H_
