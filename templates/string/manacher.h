#ifndef MANACHER_H_
#define MANACHER_H_

#include <string>
#include <vector>

template <typename Seq>
std::vector<int> Manacher(const Seq &s) {
  std::vector<int> t;
  std::vector<int> p;
  t.reserve(s.size() * 2 + 1);
  t.push_back(-1);
  for (int i = 0; i < s.size(); i++) {
    t.push_back(s[i]);
    if (i < s.size() - 1) t.push_back(-2);
  }
  t.push_back(-3);
  p.resize(t.size());
  p[0] = 0;
  int now = 0, right = 0;
  for (int i = 1; i < p.size() - 1; i++) {
    if (right > i) {
      p[i] = std::min(p[now * 2 - i], right - i);
    } else {
      p[i] = 0;
    }
    while (t[i - p[i] - 1] == t[i + p[i] + 1]) p[i]++;
    if (i + p[i] > right) {
      now = i;
      right = i + p[i];
    }
  }
  t.resize(s.size() * 2 - 1);
  for (int i = 0; i < s.size(); i++) {
    t[i * 2] = p[i * 2 + 1] / 2;
    if (i < s.size() - 1) {
      t[i * 2 + 1] = (p[i * 2 + 2] + 1) / 2;
    }
  }
  return t;
}

#endif  // MANACHER_H_
