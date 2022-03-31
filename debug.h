#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#define debug(x) std::cout << #x << ": " << (x) << endl

template <typename T, size_t size>
std::ostream& operator<<(std::ostream& os, const std::array<T, size>& a);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& a);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& a);

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& a);

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& a);

template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& a);

template <std::size_t...>
struct index_seq {};

template <std::size_t N, std::size_t... Is>
struct make_index_seq : make_index_seq<N - 1, N - 1, Is...> {};

template <std::size_t... Is>
struct make_index_seq<0, Is...> : index_seq<Is...> {};

template <typename T, size_t size>
std::ostream& operator<<(std::ostream& os, const std::array<T, size>& a) {
  os << "array[";
  for (int i = 0; i < size; i++) {
    os << a[i];
    if (i < size - 1) os << ", ";
  }
  os << "]";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& a) {
  os << "vector[";
  for (int i = 0; i < a.size(); i++) {
    os << a[i];
    if (i < a.size() - 1) os << ", ";
  }
  os << "]";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& a) {
  os << "set[";
  int cnt = 0;
  for (const T& x : a) {
    os << x;
    cnt++;
    if (cnt < a.size()) os << ", ";
  }
  os << "]";
  return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& a) {
  os << "map{";
  for (auto it = a.begin(); it != a.end(); it++) {
    os << it->first << " -> " << it->second;
    if (it != --a.end()) {
      os << ", ";
    }
  }
  os << "}";
  return os;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& a) {
  os << "pair(" << a.first << ", " << a.second << ")";
  return os;
}

namespace tuple_helper {

template <typename T>
void PrintTupleElements(std::ostream& os, const T& t) {
  os << t;
}

template <typename T, typename... Args>
void PrintTupleElements(std::ostream& os, const T& t, const Args&... rest) {
  os << t << ", ";
  PrintTupleElements(os, rest...);
}

template <typename T, std::size_t... Is>
void PrintTuple(std::ostream& os, const T& a, index_seq<Is...> s) {
  PrintTupleElements(os, std::get<Is>(a)...);
}

}  // namespace tuple_helper

template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& a) {
  os << "tuple(";
  auto seq = make_index_seq<sizeof...(T)>();
  tuple_helper::PrintTuple(os, a, seq);
  os << ")";
  return os;
}

#endif  // DEBUG_H_