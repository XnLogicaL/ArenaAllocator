cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 23)

project(ArenaAllocator)

add_library(ArenaAllocator STATIC src/arena.cpp)
target_include_directories(ArenaAllocator PUBLIC "${PROJECT_SOURCE_DIR}/include")

project(ArenaAllocatorTest)

add_executable(ArenaAllocatorTest src/main.cpp)
target_include_directories(ArenaAllocator PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(ArenaAllocatorTest ArenaAllocator)

