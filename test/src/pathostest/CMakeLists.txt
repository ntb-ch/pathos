cmake_minimum_required(VERSION 2.8)
project(pathosPeepingPanelsTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Additional include and link directories
include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

set(SRCS_FILES pathosPeepingPanelsTest.cpp)

add_executable(pathosPeepingPanelsTest ${SRCS_FILES})
target_link_libraries(pathosPeepingPanelsTest pathos eeros)

