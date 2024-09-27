#ifndef BASIS_H_
#define BASIS_H_

template <typename T = long long, int N = 60>
struct Basis {
  T a[N];
  Basis() {
    for (int i = 0; i < N; i++) a[i] = 0;
  }
  void Insert(T x) {
    for (int i = N - 1; i >= 0; i--) {
      if (x & (T(1) << i)) {
        if (!a[i]) {
          a[i] = x;
          break;
        } else {
          x ^= a[i];
        }
      }
    }
  }
  T GetMax() {
    T x = 0;
    for (int i = N - 1; i >= 0; i--) {
      if ((x & (T(1) << i)) == 0) {
        if (a[i]) x ^= a[i];
      }
    }
    return x;
  }
};

#endif  // BASIS_H_
