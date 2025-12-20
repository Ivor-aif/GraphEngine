cmake_minimum_required(VERSION 3.25)
project(GraphEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphEngine test.cpp sources/Graph.cpp headers/Graph.h)
