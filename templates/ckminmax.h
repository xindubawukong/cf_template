#ifndef CKMINMAX_H_
#define CKMINMAX_H_

bool ckmax(auto& a, auto b) {
  if (a < b) {
    a = b;
    return true;
  }
  return false;
}

bool ckmin(auto& a, auto b) {
  if (a > b) {
    a = b;
    return true;
  }
  return false;
}

#endif  // CKMINMAX_H_
