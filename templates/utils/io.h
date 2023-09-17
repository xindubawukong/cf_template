#ifndef IO_H_
#define IO_H_

#include <iostream>
#include <string>

std::string ReadLine() {
  std::string s;
  std::getline(std::cin, s);
  return s;
}

template <unsigned int n = 1000000>
struct FastInput {
  unsigned int p1, p2;
  char buf[n];
  FastInput() : p1(0), p2(0) {}
  inline char ReadChar() {
    return p1 == p2 && (p2 = (p1 = 0) + fread(buf, 1, n, stdin), p1 == p2)
               ? EOF
               : buf[p1++];
  }
  int ReadInt() {
    int x = 0, f = 1;
    char c = ReadChar();
    while (!isdigit(c)) {
      if (c == '-') f = -1;
      c = ReadChar();
    }
    while (isdigit(c)) x = x * 10 + (c ^ 48), c = ReadChar();
    return x * f;
  }
};

template <unsigned int n = 1000000>
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
