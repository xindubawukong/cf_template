#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define COLOR_START_STR "\033[0;31m"
#define COLOR_END_STR "\033[0m"

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
    s += x;
    return s;
  }
};

template <>
struct DebugPrinter<int> {
  int x;
  operator std::string() { return std::to_string(x); }
};

template <>
struct DebugPrinter<bool> {
  bool x;
  operator std::string() { return x ? "true" : "false"; }
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
struct DebugPrinter<std::set<T>> {
  std::set<T> x;
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

template <typename A, typename B>
struct DebugPrinter<std::map<A, B>> {
  std::map<A, B> x;
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