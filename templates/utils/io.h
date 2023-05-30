#ifndef IO_H_
#define IO_H_

#include <iostream>
#include <string>

template <typename T = std::string> T ReadLine() {
  T s;
  std::getline(std::cin, s);
  return s;
}

#endif // IO_H_
