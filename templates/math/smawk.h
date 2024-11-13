#ifndef SMAWK_H_
#define SMAWK_H_

#include <vector>

namespace smawk_internal {

template <typename F>
std::vector<int> RowMin(F f, const std::vector<int>& row,
                        const std::vector<int>& col) {
  int n = row.size(), m = col.size();
  auto A = [&](int i, int j) { return f(row[i], col[j]); };
  std::vector<int> res(n);
  if (n == 1) {
    res[0] = 0;
    for (int j = 1; j < m; j++) {
      if (A(0, j) < A(0, res[0])) res[0] = j;
    }
    return res;
  }
  std::vector<int> b(m);
  for (int i = 0; i < m; i++) b[i] = i;
  if (n < m) {
    b = {0};
    for (int i = 1; i < m; i++) {
      int k = b.size();
      if (k > 0 && A(k - 1, b.back()) >= A(k - 1, i)) {
        b.pop_back();
        i--;
      } else if (k < n) {
        b.push_back(i);
      }
    }
  }
  std::vector<int> row_2, col_2;
  for (int i = 0; i < n; i += 2) row_2.push_back(row[i]);
  for (int t : b) col_2.push_back(col[t]);
  auto res_2 = RowMin(f, row_2, col_2);
  for (int i = 0; i < n; i += 2) {
    res[i] = b[res_2[i / 2]];
  }
  for (int i = 1; i < n; i += 2) {
    int l = res[i - 1];
    int r = i == n - 1 ? m - 1 : res[i + 1];
    res[i] = l;
    for (int j = l + 1; j <= r; j++) {
      if (A(i, j) < A(i, res[i])) res[i] = j;
    }
  }
  return res;
}

template <typename F>
std::vector<int> RowMax(F f, const std::vector<int>& row,
                        const std::vector<int>& col) {
  int n = row.size(), m = col.size();
  auto A = [&](int i, int j) { return f(row[i], col[j]); };
  std::vector<int> res(n);
  if (n == 1) {
    res[0] = 0;
    for (int j = 1; j < m; j++) {
      if (A(0, j) > A(0, res[0])) res[0] = j;
    }
    return res;
  }
  std::vector<int> b(m);
  for (int i = 0; i < m; i++) b[i] = i;
  if (n < m) {
    b = {0};
    for (int i = 1; i < m; i++) {
      int k = b.size();
      if (k > 0 && A(n - k, b.back()) <= A(n - k, i)) {
        b.pop_back();
        i--;
      } else if (k < n) {
        b.push_back(i);
      }
    }
  }
  std::vector<int> row_2, col_2;
  for (int i = 0; i < n; i += 2) row_2.push_back(row[i]);
  for (int t : b) col_2.push_back(col[t]);
  auto res_2 = RowMax(f, row_2, col_2);
  for (int i = 0; i < n; i += 2) {
    res[i] = b[res_2[i / 2]];
  }
  for (int i = 1; i < n; i += 2) {
    int l = i == n - 1 ? 0 : res[i + 1];
    int r = res[i - 1];
    res[i] = l;
    for (int j = l + 1; j <= r; j++) {
      if (A(i, j) > A(i, res[i])) res[i] = j;
    }
  }
  return res;
}

}  // namespace smawk_internal

// Return the column indices of row minimum elements.
// Matrix A is convex totally monotone in row:
// For a < b and c < d, if A(a,c) >= A(a,d), then A(b,c) >= A(b,d).
template <typename A>
std::vector<int> SMAWK_RowMin(int n, int m, A a) {
  std::vector<int> row(n), col(m);
  for (int i = 0; i < n; i++) row[i] = i;
  for (int j = 0; j < m; j++) col[j] = j;
  return smawk_internal::RowMin(a, row, col);
}

// TBD: check if this is correct
// Return the column indices of row maximum elements.
// Matrix A is convex totally monotone in row:
// For a < b and c < d, if A(a,c) >= A(a,d), then A(b,c) >= A(b,d).
template <typename A>
std::vector<int> SMAWK_RowMax(int n, int m, A a) {
  std::vector<int> row(n), col(m);
  for (int i = 0; i < n; i++) row[i] = i;
  for (int j = 0; j < m; j++) col[j] = j;
  return smawk_internal::RowMax(a, row, col);
}

#endif  // SMAWK_H_
