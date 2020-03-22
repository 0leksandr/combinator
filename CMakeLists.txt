cmake_minimum_required(VERSION 3.9)
project(Combinator)

set(CMAKE_CXX_STANDARD 11)

add_executable(Combinator main.cpp Combinator.cpp Combinator.h)

include_directories("/home/nezhraba/ClionProjects/myLibs")
include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0")
