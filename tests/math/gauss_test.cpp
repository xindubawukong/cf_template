#include "math/gauss.h"

#include <random>
#include <vector>

#include "gtest/gtest.h"

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST(GaussTest, BasicTest) {
  int n = 100;
  std::vector<double> x(n);
  for (int i = 0; i < n; i++) {
    x[i] = rng() % 100 + 1;
  }
  std::vector<std::vector<double>> a(n);
  for (int i = 0; i < n; i++) {
    a[i].resize(n + 1);
    for (int j = 0; j < n; j++) {
      a[i][j] = rng() % 10 + 1;
      a[i][n] += a[i][j] * x[j];
    }
  }
  auto y = Gauss(n, a);
  for (int i = 0; i < n; i++) {
    EXPECT_TRUE(abs(x[i] - y[i]) < 1e-5);
  }
}