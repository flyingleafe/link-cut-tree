cmake_minimum_required(VERSION 3.9.0)
project(link_cut_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -std=c++17 -Wwrite-strings")

set(SOURCES
    src/link_cut_tree.cpp)

# add_executable(libtcpflf ${SOURCE_FILES_TCP})
add_executable(splay_test splay_test.cpp)
