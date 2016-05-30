cmake_minimum_required(VERSION 2.8)
project(can-eeros-test)

find_package(EEROS REQUIRED)
include_directories(${EEROS_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR})

find_package(flink REQUIRED)
include_directories(${FLINK_INCLUDE_DIR})
link_directories(${FLINK_LIB_DIR})

# Additional include and link directories
include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(can_test_eeros main_can_test.cpp
				  control/CanControlSystem.cpp
				  control/CanHandle.cpp
				  control/CanReceiveFaulhaber.cpp
				  control/CanSendFaulhaber.cpp
				  
				  sequences/CanTestMainSequence.cpp
				  safety/CanSafetyProperties.cpp
		       )

target_link_libraries(can_test_eeros eeros pathos canopen)
INSTALL(TARGETS can_test_eeros RUNTIME DESTINATION bin)

add_executable(can_path_eeros main_can_path.cpp
				  control/CanControlSystem.cpp
				  control/CanHandle.cpp
				  control/CanReceiveFaulhaber.cpp
				  control/CanSendFaulhaber.cpp
				  
				  sequences/CanPathMainSequence.cpp
				  safety/CanSafetyProperties.cpp
		       )

target_link_libraries(can_path_eeros eeros pathos canopen)
INSTALL(TARGETS can_path_eeros RUNTIME DESTINATION bin)