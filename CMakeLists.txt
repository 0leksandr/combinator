project(Combinator)
cmake_minimum_required(VERSION 3.17)

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(Combinator main.cpp Combinator.h)

include_directories("/home/nezhraba/ClionProjects/myLibs")

##include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0/libs/assert")
#include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0/boost/multi_array.hpp")
##include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0/boost")
##include_directories("/home/nezhraba/_/C++/third_party/linux/boost/boost_1_61_0")
#
#target_link_libraries(
#        Combinator
#        ${Boost_PROGRAM_OPTIONS_LIBRARY}
#)
