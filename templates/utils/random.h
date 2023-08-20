#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

#endif  // RANDOM_H_
