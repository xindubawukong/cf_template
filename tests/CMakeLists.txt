include(GoogleTest)

function(add_simple_test NAME)
  add_executable(${NAME} ${NAME}.cpp)
  target_link_libraries(${NAME} gtest_main)
  gtest_discover_tests(${NAME})
endfunction()

add_subdirectory(ds)
add_subdirectory(graph)
add_subdirectory(math)
add_subdirectory(string)
add_subdirectory(tree)
add_subdirectory(utils)

add_simple_test(geometry_test)
add_simple_test(range_min_test)
add_simple_test(tpoint_test tpoint_test.cpp)
