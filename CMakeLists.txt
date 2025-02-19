cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 23)

project(ArenaAllocator)

add_library(ArenaAllocator SHARED ./src/arena.cpp)
target_include_directories(ArenaAllocator PUBLIC ./include)
