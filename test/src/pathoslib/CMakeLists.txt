cmake_minimum_required(VERSION 2.8)
project(pathoslib)

include_directories(include ${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_library(pathos Hardware.cpp)
target_link_libraries(pathos eeros flink)
