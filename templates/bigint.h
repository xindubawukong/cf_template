#ifndef BIGINT_H_
#define BIGINT_H_

#include <iostream>
#include <string>
#include <vector>

template <typename T, T base>
struct BigInt : public std::vector<T> {
  static_assert(std::is_unsigned<T>::value);
  static_assert(base > 0);
  char sign;
  BigInt(T x = 0) : std::vector<T>({x}) {
    sign = 1;
    Normalize();
  }
  BigInt(const BigInt<T, base>& b) : std::vector<T>(b), sign(b.sign) {}
  BigInt(BigInt<T, base>&& b) : std::vector<T>(std::move(b)), sign(b.sign) {}
  BigInt<T, base>& operator=(const BigInt<T, base>& b) {
    sign = b.sign;
    std::vector<T>::operator=(b);
    return *this;
  }
  BigInt<T, base>& operator=(BigInt<T, base>&& b) {
    sign = b.sign;
    std::vector<T>::operator=(std::move(b));
    return *this;
  }

  operator std::string() const {
    std::string t = to_string(base - 1);
    std::string s = sign == 1 ? "" : "-";
    for (int i = this->size() - 1; i >= 0; i--) {
      std::string now = to_string((*this)[i]);
      if (i < (int)this->size() - 1) {
        while (now.length() < t.length()) now = '0' + now;
      }
      s += now;
    }
    return s;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigInt<T, base>& a) {
    os << static_cast<std::string>(a);
    return os;
  }

  friend std::istream& operator>>(std::istream& is, BigInt<T, base>& a) {
    static_assert(base == 10 || base == 100 || base == 1000 || base == 10000 ||
                  base == 100000 || base == 1000000);
    std::string s;
    is >> s;
    if (s[0] == '-') {
      a.sign = -1;
      s = s.substr(1);
    } else {
      a.sign = 1;
    }
    int len = to_string(base - 1).length();
    std::reverse(s.begin(), s.end());
    a.clear();
    for (int i = 0; i < (int)s.length(); i += len) {
      T x = 0;
      for (int j = std::min((int)s.length() - 1, i + len - 1); j >= i; j--) {
        x = x * 10 + s[j] - '0';
      }
      a.push_back(x);
    }
    a.Normalize();
    return is;
  }

  void Normalize() {
    for (int i = 0; i < (int)this->size(); i++) {
      if ((*this)[i] >= base) {
        if (i == (int)(*this).size() - 1) (*this).push_back(0);
        (*this)[i + 1] += (*this)[i] / base;
        (*this)[i] %= base;
      }
    }
    while (this->size() > 1 && this->back() == 0) this->pop_back();
    if (this->size() == 1 && (*this)[0] == 0) sign = 1;
  }

  BigInt<T, base> Abs() const {
    auto x = *this;
    x.sign = 1;
    return x;
  }

  bool operator<(const BigInt<T, base>& b) const {
    if (sign != b.sign) return sign < b.sign;
    int n = this->size(), m = b.size();
    if (sign == 1) {
      if (n < m) return true;
      if (n > m) return false;
      for (int i = n - 1; i >= 0; i--) {
        if ((*this)[i] < b[i]) return true;
        if ((*this)[i] > b[i]) return false;
      }
      return false;
    } else {
      if (n < m) return false;
      if (n > m) return true;
      for (int i = n - 1; i >= 0; i--) {
        if ((*this)[i] < b[i]) return false;
        if ((*this)[i] > b[i]) return true;
      }
      return false;
    }
  }
  bool operator<=(const BigInt<T, base>& b) const { return !(b < *this); }
  bool operator>(const BigInt<T, base>& b) const { return b < *this; }
  bool operator>=(const BigInt<T, base>& b) const { return b <= *this; }
  bool operator==(const BigInt<T, base>& b) const {
    if (sign != b.sign) return false;
    return std::vector<T>::operator==(b);
  }
  bool operator!=(const BigInt<T, base>& b) const { return !((*this) == b); }

  friend BigInt<T, base> operator+(const BigInt<T, base>& a,
                                   const BigInt<T, base>& b) {
    if (a.sign == b.sign) {
      int n = a.size(), m = b.size();
      int len = std::max(n, m);
      BigInt<T, base> c;
      c.resize(len);
      c.sign = a.sign;
      for (int i = 0; i < len; i++) {
        c[i] = (i < n ? a[i] : 0) + (i < m ? b[i] : 0);
      }
      c.Normalize();
      return c;
    } else {
      BigInt<T, base> c = a.Abs(), d = b.Abs(), e;
      if (c < d) {
        e = d - c;
        e.sign = b.sign;
      } else {
        e = c - d;
        e.sign = a.sign;
      }
      e.Normalize();
      return e;
    }
  }

  friend BigInt<T, base> operator-(const BigInt<T, base>& a,
                                   const BigInt<T, base>& b) {
    BigInt<T, base> c = a.Abs(), d = b.Abs();
    if (a.sign == b.sign) {
      char t = a.sign;
      if (c < d) {
        swap(c, d);
        t *= -1;
      }
      int n = a.size(), m = b.size();
      BigInt<T, base> e;
      e.resize(n);
      for (int i = 0; i < n; i++) {
        T q = i < m ? d[i] : 0;
        if (c[i] >= q) {
          e[i] = c[i] - q;
        } else {
          int id = -1;
          for (int j = i + 1; j < n; j++) {
            if (c[j] > 0) {
              id = j;
              break;
            }
          }
          assert(id != -1);
          c[id]--;
          for (int j = id - 1; j > i; j--) c[j] = 9;
          c[i] += 10;
          e[i] = c[i] - q;
        }
      }
      e.sign = t;
      e.Normalize();
      return e;
    } else {
      auto e = c + d;
      e.sign = a.sign;
      return e;
    }
  }

  // BigInt operator*(int x) const {
  //   int n = a.size();
  //   auto b = *this;
  //   for (int i = 0; i < n; i++) {
  //     b.a[i] *= x;
  //   }
  //   b.Normalize();
  //   return b;
  // }

  // friend BigInt operator*(int x, const BigInt<T, base>& a) { return a * x; }

  // BigInt operator*(const BigInt<T, base>& b) const {
  //   int n = a.size(), m = b.a.size();
  //   BigInt c;
  //   c.a.resize(n + m);
  //   for (int i = 0; i < n; i++) {
  //     for (int j = 0; j < m; j++) {
  //       c.a[i + j] += a[i] * b.a[j];
  //     }
  //   }
  //   c.Normalize();
  //   return c;
  // }

  // // return a/b and a%b
  // std::pair<BigInt, BigInt> operator/(const BigInt<T, base>& b) const {
  //   BigInt ans = *this, rest = 0;
  //   for (int i = a.size() - 1; i >= 0; i--) {
  //     rest = rest * base + a[i];
  //     T x;
  //     if (rest >= b) {
  //       x = 1;
  //       // find max x that b * x <= rest
  //       T l = 2, r = base;
  //       while (l <= r) {
  //         T m = (l + r) / 2;
  //         if (b * m <= rest) {
  //           x = m;
  //           l = m + 1;
  //         } else {
  //           r = m - 1;
  //         }
  //       }
  //     } else {
  //       x = 0;
  //     }
  //     ans.a[i] = x;
  //     rest = rest - b * x;
  //   }
  //   ans.Normalize();
  //   return make_pair(ans, rest);
  // }
};

#endif  // BIGINT_H_
