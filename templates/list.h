#ifndef LIST_H_
#define LIST_H_

template <typename T>
struct List {
  struct Node {
    Node *next, *prev;
    T val;
    Node(T val_) : next(nullptr), prev(nullptr), val(val_) {}
  };
  Node *head, *tail;
  int size;
  List() : head(nullptr), tail(nullptr), size(0) {}
  T& Back() {
    assert(size > 0);
    return tail->val;
  }
  T Back() const {
    assert(size > 0);
    return tail->val;
  }
  T& Front() {
    assert(size > 0);
    return head->val;
  }
  T Front() const {
    assert(size > 0);
    return head->val;
  }
  Node* InsertBefore(Node* it, T val) {
    assert(it != nullptr);
    size++;
    auto x = new Node(val);
    auto p = it->prev;
    if (p) {
      p->next = x;
      x->prev = p;
    }
    x->next = it;
    it->prev = x;
    if (it == head) head = x;
    return x;
  }
  Node* InsertAfter(Node* it, T val) {
    assert(it != nullptr);
    size++;
    auto x = new Node(val);
    auto n = it->next;
    if (n) {
      n->prev = x;
      x->next = n;
    }
    x->prev = it;
    it->next = x;
    if (it == tail) tail = x;
    return x;
  }
  void Delete(Node* it) {
    assert(it != nullptr);
    size--;
    auto p = it->prev, n = it->next;
    if (p) p->next = n;
    if (n) n->prev = p;
    if (head == it) head = n;
    if (tail == it) tail = p;
    delete it;
  }
  void PushBack(T val) {
    if (head == nullptr) {
      head = tail = new Node(val);
      size = 1;
    } else {
      InsertAfter(tail, val);
    }
  }
  void PushFront(T val) {
    if (head == nullptr) {
      head = tail = new Node(val);
      size = 1;
    } else {
      InsertBefore(head, val);
    }
  }
  void PopBack() {
    assert(size > 0);
    Delete(tail);
  }
  void PopFront() {
    assert(size > 0);
    Delete(head);
  }
  void Clear() {
    while (size > 0) Delete(head);
  }
};

#endif  // LIST_H_
