cmake_minimum_required(VERSION 3.15.0)
project(XdbwkCfTemplate VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-DLOCAL)

add_executable(main main.cpp)

file(GLOB problems true problem_*)

foreach(problem ${problems})
  add_subdirectory(${problem})
endforeach()
