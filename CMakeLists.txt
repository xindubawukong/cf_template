cmake_minimum_required(VERSION 3.0.0)
project(cccfff VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME )
set(CPACK_PROJECT_VERSION )
include(CPack)

set(CMAKE_CXX_STANDARD 20)

add_executable(template template.cpp debug.h debug.cpp)

add_executable(problem_A problem_A/A.cpp debug.h debug.cpp)
