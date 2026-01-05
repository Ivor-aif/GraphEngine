cmake_minimum_required(VERSION 3.25)
project(GraphEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(GraphEngine main.cpp tests/test.cpp
        sources/Graph.cpp headers/Graph.h
        sources/algorithm.cpp headers/algorithm.h
        sources/generator.cpp headers/generator.h
        sources/stats.cpp headers/stats.h
)
