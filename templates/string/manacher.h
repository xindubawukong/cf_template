#ifndef MANACHER_H_
#define MANACHER_H_

#include <algorithm>
#include <string>
#include <vector>

//    s: "a a b a a a a"
//  odd: [0,0,2,0,1,1,0]
// even: [1,0,0,1,2,1]
template <typename Seq>
std::pair<std::vector<int>, std::vector<int>> Manacher(const Seq& s) {
  int n = s.size();
  assert(*std::min_element(s.begin(), s.end()) >= 0);
  std::vector<int> t;
  std::vector<int> p;
  t.reserve(n * 2 + 1);
  t.push_back(-1);
  for (int i = 0; i < n; i++) {
    t.push_back(s[i]);
    if (i < n - 1) t.push_back(-2);
  }
  t.push_back(-3);
  p.resize(t.size());
  p[0] = 0;
  int now = 0, right = 0;
  for (int i = 1; i < (int)p.size() - 1; i++) {
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
  std::vector<int> odd(n), even(n - 1);
  for (int i = 0; i < n; i++) {
    odd[i] = p[i * 2 + 1] / 2;
    if (i < n - 1) {
      even[i] = (p[i * 2 + 2] + 1) / 2;
    }
  }
  return {odd, even};
}

#endif  // MANACHER_H_
