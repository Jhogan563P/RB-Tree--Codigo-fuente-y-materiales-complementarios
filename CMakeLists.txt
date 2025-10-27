cmake_minimum_required(VERSION 3.29)
project(Benchmarking_CLion)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

add_executable(Benchmarking_CLion main.cpp)
