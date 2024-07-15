cmake_minimum_required(VERSION 3.15.0)
project(
  XdbwkCfTemplate
  VERSION 0.3.0
  DESCRIPTION "XdbwkCfTemplate"
  HOMEPAGE_URL "https://github.com/xindubawukong/cf_template"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-DLOCAL)

set(WarningOptions
-O2
-Wall -Wextra -pedantic
-Wshadow -Wformat=2 -Wfloat-equal -Wcast-qual -Wcast-align
-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
-fsanitize=address
-fno-sanitize-recover
-fstack-protector
)
add_compile_options(${WarningOptions})
add_link_options(${WarningOptions})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(templates)
include_directories(utils)

enable_testing()
add_subdirectory(googletest)
# add_subdirectory(parlaylib)
add_subdirectory(tests)

file(GLOB problems true *.cpp)
foreach(problem ${problems})
  string(LENGTH ${problem} len)
  string(FIND ${problem} "/" pos REVERSE)
  math(EXPR pos "${pos}+1")
  math(EXPR len "${len}-${pos}-4")
  string(SUBSTRING ${problem} ${pos} ${len} name)
  add_executable(${name} ${name}.cpp)
endforeach()
