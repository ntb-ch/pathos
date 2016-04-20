cmake_minimum_required(VERSION 2.8)
project(canopenlib)

# Additional include and link directories
include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

add_subdirectory(lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#link_directories(${CANOPEN_BINARY_DIR}/lib)
add_library(canopen lib/canopen.c lib/canopen-com.c lib/can-if.c lib/canopen-drv.c lib/canopen-faulhaber-drv.c)
