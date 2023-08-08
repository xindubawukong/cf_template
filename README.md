## CF-Template

https://github.com/xindubawukong/cf_template

Templates for writing competitive programming problems in C++.

## Dependencies

- Cmake >= 3.15
- gcc 12 or clang 14
- googletest and parlaylib (included as git submodules)

## Usage

### Download

```
git clone --recurse-submodules git@github.com:xindubawukong/cf_template.git
```

### Run a Problem

```shell
$ ./go A  # compile problem_A and run (if not created, create first)
$ ./go B  # problem_B
$ ./go A B  # compile and run problem_A and problem_B
$ ./go A B --no-build  # run without compile
$ ./go clean  # delete all problem_ directories
$ ./go clean A  # delete problem_A
```

You can directly include and use the templates in `A.cpp`:

```C++
#include "ds/segment_tree.h"
#include "math/min.h"
```

After `./go A`, you should submit `submit_problem_A.cpp` (not `A.cpp`), which includes the source code of all the templates you used.

### Debug

`debug.h` is included if running locally. You can directly debug almost everything:

```C++
int x = 1;
vector<pair<char, string>> a(10);
map<int, tuple<int, int>> b;
debug(x, a, b);
```

For custom structs, you can debug it if it can be auto converted to `std::string`.

### Run Unit Tests

```shell
$ cd build
$ cmake ..
$ make
$ ctest
```

## Links

- <a href="https://clist.by/coder/xindubawukong/">xindubawukong (CLIST)</a>
- <a href="https://codeforces.com/profile/xindubawukong">xindubawukong (Codeforces)</a>
- <a href="https://atcoder.jp/users/xindubawukong">xindubawukong (AtCoder)</a>
