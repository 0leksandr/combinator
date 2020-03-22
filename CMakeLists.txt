project(Combinator)
cmake_minimum_required(VERSION 3.17)

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(Combinator main.cpp Combinator.cpp Combinator.h)

include_directories("/home/nezhraba/ClionProjects/myLibs")
include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0/libs/assert")
