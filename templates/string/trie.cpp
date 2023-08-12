struct Node {
  vector<Node*> son;
  int size;
  Node() : son(2, nullptr), size(0) {}
};

Node* Insert(Node* x, int val, int dep) {
  if (!x) x = new Node();
  x->size++;
  if (dep == -1) return x;
  int t = (val >> dep) & 1;
  x->son[t] = Insert(x->son[t], val, dep - 1);
  return x;
}

Node* Erase(Node* x, int val, int dep) {
  x->size--;
  if (dep == -1) return x;
  int t = (val >> dep) & 1;
  x->son[t] = Erase(x->son[t], val, dep - 1);
  return x;
}