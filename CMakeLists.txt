cmake_minimum_required(VERSION 3.15.0)
project(XdbwkCfTemplate VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-DLOCAL)

file(GLOB Problems true problem_*)
# message("Problems: ${Problems}")

foreach(Problem ${Problems})
  # message("Problem: ${Problem}")
  add_subdirectory(${Problem})
endforeach()
