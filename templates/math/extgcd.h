#ifndef EXTGCD_H_
#define EXTGCD_H_

template <typename T>
T Extgcd(T a, T b, T& x, T& y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  T d = Extgcd(b, a % b, x, y);
  T t = x;
  x = y;
  y = t - (a / b) * y;
  return d;
}

#endif  // EXTGCD_H_
