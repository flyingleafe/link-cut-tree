cmake_minimum_required(VERSION 3.9.0)
project(link_cut_tree)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES
    src/splay_node.cpp
    src/splay_tree.cpp
    src/link_cut_tree.cpp)

add_executable(splay_test splay_test.cpp ${SOURCES})
add_executable(link_cut_test link_cut_test.cpp ${SOURCES})
