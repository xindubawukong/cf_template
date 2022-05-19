#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// https://xdevs.com/guide/color_serial/
const std::string COLOR_START_STR = "\033[0;31m";
const std::string COLOR_END_STR = "\033[0m";

#define debug(...)                                     \
  std::cout << COLOR_START_STR << "[ " << #__VA_ARGS__ \
            << " ]: " << COLOR_END_STR,                \
      DebugPrint(__VA_ARGS__)

template <typename T>
struct DebugPrinter {
  T x;
  operator std::string() { return static_cast<std::string>(x); }
};

template <typename A>
void DebugPrint(A a) {
  std::cout << COLOR_START_STR << static_cast<std::string>(DebugPrinter<A>({a}))
            << COLOR_END_STR << std::endl;
}

template <typename A, typename... B>
void DebugPrint(A a, B... b) {
  std::cout << COLOR_START_STR << static_cast<std::string>(DebugPrinter<A>({a}))
            << ", " << COLOR_END_STR;
  DebugPrint(b...);
}

// DebugPrinter specializations

template <>
struct DebugPrinter<char> {
  char x;
  operator std::string() {
    std::string s;
    s += '\'';
    s += x;
    s += '\'';
    return s;
  }
};

template <>
struct DebugPrinter<short> {
  short x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<int> {
  int x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<long> {
  long x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<unsigned int> {
  unsigned int x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<unsigned long> {
  unsigned long x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<long long> {
  long long x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<unsigned long long> {
  unsigned long long x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<bool> {
  bool x;
  operator std::string() { return x ? "true" : "false"; }
};

template <>
struct DebugPrinter<float> {
  float x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<double> {
  double x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<long double> {
  long double x;
  operator std::string() { return std::to_string(x); }
};

template <size_t N>
struct DebugPrinter<std::bitset<N>> {
  std::bitset<N> x;
  operator std::string() {
    std::string s = "bitset<" + std::to_string(N) + ">(";
    s += x.to_string();
    s += ")";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::optional<T>> {
  std::optional<T> x;
  operator std::string() {
    std::string s = "optional(";
    if (x.has_value()) {
      s += static_cast<std::string>(DebugPrinter<T>({x.value()}));
    }
    s += ")";
    return s;
  }
};

template <typename A, typename B>
struct DebugPrinter<std::pair<A, B>> {
  std::pair<A, B> x;
  operator std::string() {
    std::string s = "pair(";
    s += static_cast<std::string>(DebugPrinter<A>({x.first}));
    s += ", ";
    s += static_cast<std::string>(DebugPrinter<B>({x.second}));
    s += ")";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::vector<T>> {
  std::vector<T> x;
  operator std::string() {
    std::string s = "vector[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::stack<T>> {
  std::stack<T> x;
  operator std::string() {
    std::string s = "stack[";
    std::vector<T> a;
    while (!x.empty()) {
      T t = x.top();
      a.push_back(t);
      x.pop();
    }
    for (int i = a.size() - 1; i >= 0; i--) {
      T t = a[i];
      if (i != a.size() - 1) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({t}));
      x.push(t);
    }
    s += "]";
    return s;
  }
};

template <typename T, std::size_t N>
struct DebugPrinter<std::array<T, N>> {
  std::array<T, N> x;
  operator std::string() {
    std::string s = "array[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::queue<T>> {
  std::queue<T> x;
  operator std::string() {
    std::string s = "queue[";
    int n = x.size();
    for (int i = 0; i < n; i++) {
      auto t = x.front();
      if (i > 0) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({t}));
      x.pop();
      x.push(t);
    }
    s += "]";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::deque<T>> {
  std::deque<T> x;
  operator std::string() {
    std::string s = "deque[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename T, typename Cmp>
struct DebugPrinter<std::set<T, Cmp>> {
  std::set<T, Cmp> x;
  operator std::string() {
    std::string s = "set[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::unordered_set<T>> {
  std::unordered_set<T> x;
  operator std::string() {
    std::string s = "unordered_set[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename T>
struct DebugPrinter<std::multiset<T>> {
  std::multiset<T> x;
  operator std::string() {
    std::string s = "multiset[";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<T>({*it}));
    }
    s += "]";
    return s;
  }
};

template <typename A, typename B, typename Cmp>
struct DebugPrinter<std::map<A, B, Cmp>> {
  std::map<A, B, Cmp> x;
  operator std::string() {
    std::string s = "map{";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<A>({it->first}));
      s += " -> ";
      s += static_cast<std::string>(DebugPrinter<B>({it->second}));
    }
    s += "}";
    return s;
  }
};

template <typename A, typename B>
struct DebugPrinter<std::unordered_map<A, B>> {
  std::unordered_map<A, B> x;
  operator std::string() {
    std::string s = "unordered_map{";
    for (auto it = x.begin(); it != x.end(); it++) {
      if (it != x.begin()) s += ", ";
      s += static_cast<std::string>(DebugPrinter<A>({it->first}));
      s += " -> ";
      s += static_cast<std::string>(DebugPrinter<B>({it->second}));
    }
    s += "}";
    return s;
  }
};

namespace tuple_helper {

template <std::size_t...>
struct index_seq {};

template <std::size_t N, std::size_t... Is>
struct make_index_seq : make_index_seq<N - 1, N - 1, Is...> {};

template <std::size_t... Is>
struct make_index_seq<0, Is...> : index_seq<Is...> {};

template <typename T>
std::string PrintTupleElements(const T& t) {
  return DebugPrinter<T>({t});
}

template <typename T, typename... Args>
std::string PrintTupleElements(const T& t, const Args&... rest) {
  std::string s = static_cast<std::string>(DebugPrinter<T>({t})) + ", ";
  s += PrintTupleElements(rest...);
  return s;
}

template <typename T, std::size_t... Is>
std::string PrintTuple(const T& a, index_seq<Is...> s) {
  return PrintTupleElements(std::get<Is>(a)...);
}

}  // namespace tuple_helper

template <typename... T>
struct DebugPrinter<std::tuple<T...>> {
  std::tuple<T...> x;
  operator std::string() {
    std::string s = "tuple(";
    auto seq = tuple_helper::make_index_seq<sizeof...(T)>();
    s += tuple_helper::PrintTuple(x, seq);
    s += ")";
    return s;
  }
};

#endif  // DEBUG_H_