cmake_minimum_required(VERSION 2.8)
project(decoy)

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

add_executable(decoy main_decoy.cpp
			control/ControlSystem_decoy.cpp
			control/CanHandle.cpp
			control/CanReceiveFaulhaber.cpp
			control/CanSendFaulhaber.cpp
			control/RadToPulses.cpp
			
			sequences/MainSequence_decoy.cpp
			sequences/Homing.cpp
			sequences/ArmMotion.cpp
			safety/SafetyProperties_decoy.cpp
		       )

target_link_libraries(decoy eeros pathos canopen)
INSTALL(TARGETS decoy RUNTIME DESTINATION bin)
