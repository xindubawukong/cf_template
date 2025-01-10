#ifndef PREFIX_SUM_H_
#define PREFIX_SUM_H_

#include <vector>

template <typename T>
struct PrefixSum {
  std::vector<T> sum;
  PrefixSum(auto& a) {
    sum.resize(a.size());
    sum[0] = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
      sum[i] = sum[i - 1] + a[i];
    }
  }
  T Sum(int l, int r) { return l == 0 ? sum[r] : sum[r] - sum[l - 1]; }
};

#endif  // PREFIX_SUM_H_
