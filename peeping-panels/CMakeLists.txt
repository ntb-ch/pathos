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

add_executable(configExample configExample.cpp)
target_link_libraries(configExample eeros pthread)

# Executable, teaching
add_executable(teach main_teach.cpp
                     control/ControlSystem_Peep.cpp     # needed, not used 
                     control/ControlSystem_Teach.cpp  
                     control/PathPlanner.cpp
                     control/ReadSensorData.cpp         # needed, not used 
                     sensors/SensorsThread.cpp          # needed, not used 
                     safety/SafetyProperties_Teach.cpp
                     sequences/MainSequence_teach.cpp        
                     sequences/Teaching.cpp   
               )
target_link_libraries(teach eeros pathos)
INSTALL(TARGETS teach RUNTIME DESTINATION bin)

# Executable, peeping
add_executable(peep main_peep.cpp  
                                   control/ControlSystem_Peep.cpp  
                                   control/ControlSystem_Teach.cpp    # needed, not used
                                   control/PathPlanner.cpp
                                   control/ReadSensorData.cpp
                                   sensors/SensorsThread.cpp
                                   safety/SafetyProperties_Peep.cpp
                                   sequences/MainSequence_peep.cpp
                                   sequences/Peep_out.cpp   
                                   sequences/Peep_in.cpp                                                              
              )   
target_link_libraries(peep eeros pathos)
INSTALL(TARGETS peep RUNTIME DESTINATION bin)

# # Encoder test
# add_executable(test-enc test/encoder/testEncoder.cpp)
# target_link_libraries(test-enc eeros pathos)
# INSTALL(TARGETS test-enc RUNTIME DESTINATION bin)

# # Behaviour test
# add_executable(test-behaviour test/behaviour/testBehaviour.cpp control/PeepingPanelControlSystem.cpp  
#                                                                control/PathPlanner.cpp
#                                                                safety/PeepingPanelSafetyProperties_x4.cpp
#                                                                test/behaviour/MainSequence_behaviour.cpp
#                                                                sequences/Peep_out.cpp   
#                                                                sequences/Peep_in.cpp   
#                                                                sequences/Teaching.cpp   
#                                                                                                           )
# target_link_libraries(test-behaviour eeros pathos)
# INSTALL(TARGETS test-behaviour RUNTIME DESTINATION bin)