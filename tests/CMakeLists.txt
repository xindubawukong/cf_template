cmake_minimum_required(VERSION 3.15.0)
project(XdbwkCfTemplate VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

include(GoogleTest)

function(add_simple_test NAME)
  add_executable(${NAME} ${NAME}.cpp)
  target_link_libraries(${NAME} gtest_main)
  gtest_discover_tests(${NAME})
endfunction()

add_simple_test(dijkstra_test)
add_simple_test(dsu_test)
add_simple_test(flow_test)
add_simple_test(get_primes_test)
add_simple_test(range_min_test)
