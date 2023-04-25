cmake_minimum_required(VERSION 3.15.0)
project(
  XdbwkCfTemplate
  VERSION 0.3.0
  DESCRIPTION "XdbwkCfTemplate"
  HOMEPAGE_URL "https://github.com/xindubawukong/cf_template"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-DLOCAL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(templates)
include_directories(templates/graph)
include_directories(templates/math)
include_directories(templates/string)
include_directories(templates/tree)
include_directories(templates/utils)

option(XdbwkCfTemplate_BuildTest "" ON)
if(${XdbwkCfTemplate_BuildTest})
  enable_testing()
  add_subdirectory(googletest)
  add_subdirectory(tests)
endif()

add_subdirectory(parlaylib)

file(GLOB problems true problem_*)
foreach(problem ${problems})
  add_subdirectory(${problem})
endforeach()
