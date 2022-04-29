
cmakecontent="cmake_minimum_required(VERSION 3.0.0)
project(cccfff VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_STANDARD 20)

add_executable(template template.cpp debug.h debug.cpp)
"

if [ -z $1 ]; then
  echo "Please specify problem name or 'clean'"
  exit 0
fi

if [ $1 == "clean" ]; then
  echo "Enter yes to clean: "
  read op
  if [ $op == "yes" ];
  then
    echo "Cleaning."
    echo "$cmakecontent" > CmakeLists.txt
    now=$(date +"%Y-%m-%d-%r")
    mkdir -p ".history/$now"
    cp -r problem_* CmakeLists.txt "./.history/$now"
    rm -rf problem_*
  else
    echo "Not cleaning. Exit."
  fi
  exit 0
fi

if [ ! -d "problem_$1" ]; then
  buildstr="add_executable(problem_$1 problem_$1/$1.cpp debug.h debug.cpp)"
  echo $buildstr >> CmakeLists.txt

  mkdir "problem_$1"
  touch problem_$1/$1.in
  touch problem_$1/$1.out
  cp template.cpp problem_$1/$1.cpp
  replace="s/debug.h/..\/debug.h/g; s/input.txt/problem_$1\/$1.in/g; s/output.txt/problem_$1\/$1.out/g"
  sed -i '' "$replace"  ./problem_$1/$1.cpp
fi

mkdir -p build
cd build
cmake .. && make && ./problem_$1
cd ..