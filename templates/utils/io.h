#ifndef IO_H_
#define IO_H_

#include <iostream>
#include <string>

std::string ReadLine() {
  std::string s;
  std::getline(std::cin, s);
  return s;
}

template <int n>
struct FastInput {
  int p1, p2;
  char buf[n];
  FastInput() : p1(0), p2(0) {}
  inline char GetChar() {
    return p1 == p2 && (p2 = (p1 = 0) + fread(buf, 1, n, stdin), p1 == p2)
               ? EOF
               : buf[p1++];
  }
  int ReadInt() {
    int x = 0, f = 1;
    char c = GetChar();
    while (!isdigit(c)) {
      if (c == '-') f = -1;
      c = GetChar();
    }
    while (isdigit(c)) x = x * 10 + (c ^ 48), c = GetChar();
    return x * f;
  }
};

template <unsigned int n>
struct FastOutput {
  unsigned int p;
  char buf[n];
  FastOutput() : p(0) {}
  inline void WriteChar(char ch) {
    if (p == n) fwrite(buf, 1, n, stdout), p = 0;
    buf[p++] = ch;
  }
  void WriteInt(int x) {
    std::string s = std::to_string(x);
    for (auto ch : s) WriteChar(ch);
  }
  ~FastOutput() {
    if (p != 0) fwrite(buf, 1, p, stdout);
  }
};

#endif  // IO_H_
