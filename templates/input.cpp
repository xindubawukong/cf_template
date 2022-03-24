#include <cstdio>

using namespace std;

int ReadInt() {
  char ch = getchar();
  while (ch < '0' || ch > '9') ch = getchar();
  int x = 0;
  while (ch >= '0' && ch <= '9') {
    x = x * 10 + ch - '0';
    ch = getchar();
  }
  return x;
}

int main() {
  return 0;
}