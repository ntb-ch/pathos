cmake_minimum_required(VERSION 2.8)

project(peeping-panels)

find_package(EEROS REQUIRED)
include_directories(${EEROS_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR})

find_package(flink REQUIRED)
include_directories(${FLINK_INCLUDE_DIR})
link_directories(${FLINK_LIB_DIR})

include_directories(${ADDITIONAL_INCLUDE_DIRS})
link_directories(${ADDITIONAL_LINK_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Executable, program from 1 to 4 motors
add_executable(peeping-panels-4 main_x4.cpp  control/PeepingPanelControlSystem.cpp  
                                             safety/PeepingPanelSafetyProperties_x4.cpp
                                             sequences/MainSequence_x4.cpp
                                             control/PathPlanner.cpp
                                             sequences/Teaching.cpp    
                                             sequences/Peep_out.cpp     
                                             sequences/Peep_in.cpp             )   
target_link_libraries(peeping-panels-4 eeros pathos)
INSTALL(TARGETS peeping-panels-4 RUNTIME DESTINATION bin)


# Executable, teaching
add_executable(peeping-panels-teach main_teach.cpp control/PeepingPanelControlSystem.cpp  
                                                   safety/PeepingPanelSafetyProperties_x4.cpp
                                                   sequences/MainSequence_teach.cpp
                                                   control/PathPlanner.cpp
                                                   sequences/Teaching.cpp       )   
target_link_libraries(peeping-panels-teach eeros pathos)
INSTALL(TARGETS peeping-panels-teach RUNTIME DESTINATION bin)

# Executable, peeping
add_executable(peeping-panels-peep main_peep.cpp  control/PeepingPanelControlSystem.cpp  
                                                  safety/PeepingPanelSafetyProperties_x4.cpp
                                                  sequences/MainSequence_peep.cpp
                                                  control/PathPlanner.cpp
                                                  sequences/Peep_out.cpp   
                                                  sequences/Peep_in.cpp         )   
target_link_libraries(peeping-panels-peep eeros pathos)
INSTALL(TARGETS peeping-panels-peep RUNTIME DESTINATION bin)

# Encoder test
add_executable(test-enc test/encoder/testEncoder.cpp)
target_link_libraries(test-enc eeros pathos)
INSTALL(TARGETS test-enc RUNTIME DESTINATION bin)

# Behaviour test
add_executable(test-behaviour test/behaviour/testBehaviour.cpp control/PeepingPanelControlSystem.cpp  
                                                               control/PathPlanner.cpp
                                                               safety/PeepingPanelSafetyProperties_x4.cpp
                                                               test/behaviour/MainSequence_behaviour.cpp
                                                               sequences/Peep_out.cpp   
                                                               sequences/Peep_in.cpp   
                                                               sequences/Teaching.cpp   
                                                                                                          )
target_link_libraries(test-behaviour eeros pathos)
INSTALL(TARGETS test-behaviour RUNTIME DESTINATION bin)