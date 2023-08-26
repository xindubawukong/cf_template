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
-Wall -Wextra -pedantic -O2
-Wshadow -Wformat=2 -Wfloat-equal -Wcast-qual -Wcast-align
-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
-fsanitize=address
-fsanitize=undefined
-fno-sanitize-recover
-fstack-protector
)
add_compile_options(${WarningOptions})
add_link_options(${WarningOptions})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(templates)
include_directories(utils)

add_executable(main templates/main.cpp)

enable_testing()
add_subdirectory(googletest)
# add_subdirectory(parlaylib)
add_subdirectory(tests)

file(GLOB problems true problem_*)
foreach(problem ${problems})
  add_subdirectory(${problem})
endforeach()
