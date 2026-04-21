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

set(OPTIONS
-O2
-Wall -Wextra -pedantic
-Wshadow -Wformat=2 -Wfloat-equal -Wcast-qual -Wcast-align
-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
# -fsanitize=address
# -fno-sanitize-recover
# -fstack-protector
)
add_compile_options(${OPTIONS})
add_link_options(${OPTIONS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/parlaylib/include)
include_directories(templates)
include_directories(utils)

enable_testing()
add_subdirectory(third_party/googletest)
add_subdirectory(tests)

file(GLOB problem_entries RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS "problem_*")
foreach(problem_name ${problem_entries})
  if(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${problem_name})
    continue()
  endif()

  set(problem_main ${CMAKE_CURRENT_SOURCE_DIR}/${problem_name}/main.cpp)
  if(NOT EXISTS ${problem_main})
    continue()
  endif()

  set(problem_binary_dir ${CMAKE_CURRENT_BINARY_DIR}/${problem_name})
  set(problem_main_target ${problem_name}_main)

  add_executable(${problem_main_target} ${problem_main})
  set_target_properties(
    ${problem_main_target}
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${problem_binary_dir}
      OUTPUT_NAME main
  )

  add_custom_target(${problem_name} ALL)
  add_dependencies(${problem_name} ${problem_main_target})
endforeach()
