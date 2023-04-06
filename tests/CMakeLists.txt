cmake_minimum_required(VERSION 3.15.0)
project(XdbwkCfTemplate VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

include(GoogleTest)

function(add_simple_test NAME PATH)
  add_executable(${NAME} ${PATH})
  target_link_libraries(${NAME} gtest_main)
  gtest_discover_tests(${NAME})
endfunction()

add_simple_test(dijkstra_test dijkstra_test.cpp)
add_simple_test(dsu_test dsu_test.cpp)
add_simple_test(flow_test flow_test.cpp)
add_simple_test(get_primes_test get_primes_test.cpp)
add_simple_test(graph_test graph_test.cpp)
add_simple_test(heavy_light_decomposition_test tree/heavy_light_decomposition_test.cpp)
add_simple_test(mint_test mint_test.cpp)
add_simple_test(range_min_test range_min_test.cpp)
add_simple_test(segment_tree_test segment_tree_test.cpp)
