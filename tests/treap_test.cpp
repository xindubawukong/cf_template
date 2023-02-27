#include "treap.h"

#include "gtest/gtest.h"

struct Entry {
  using key_t = int;
  int val;
  int size;

  Entry(int val) : val(val), size(1) {}

  key_t GetKey() { return val; }

  void Update(Entry* l, Entry* r) {
    size = 1;
    if (l) size += l->size;
    if (r) size += r->size;
  }

  static bool Less(const key_t& a, const key_t& b) { return a > b; }
};

TEST(TreapTest, BasicTest) {
  treap::Treap<Entry> treap;
  treap.Insert(Entry(3));
  treap.Insert(Entry(2));
  treap.Insert(Entry(4));
  treap.Insert(Entry(1));
  EXPECT_EQ(treap.root->entry.size, 4);
  EXPECT_EQ(treap.GetKth(2).value().val, 3);
  EXPECT_EQ(treap.GetRank(2), 3);
  EXPECT_EQ(treap.GetNext(3).value().val, 2);
  EXPECT_EQ(treap.GetPrev(3).value().val, 4);
  EXPECT_FALSE(treap.GetNext(1).has_value());
  treap.Delete(3);
  EXPECT_EQ(treap.root->entry.size, 3);
  EXPECT_EQ(treap.GetKth(2).value().val, 2);
}