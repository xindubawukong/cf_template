cmake_minimum_required(VERSION 3.15.0)
project(XdbwkCfTemplate VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-DLOCAL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(XdbwkCfTemplate_BuildTest "" OFF)
if(${XdbwkCfTemplate_BuildTest})
  enable_testing()
  add_subdirectory(googletest)
  include_directories(templates)
  add_subdirectory(tests)
endif()

file(GLOB problems true problem_*)
foreach(problem ${problems})
  add_subdirectory(${problem})
endforeach()
