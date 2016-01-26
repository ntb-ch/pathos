cmake_minimum_required(VERSION 2.8)

project(peeping-panels)

include_directories(/home/eeros/efs/efs/build-arm/eeros)
include_directories(/home/eeros/efs/efs/eeros/includes)
include_directories(/home/eeros/efs/efs/flink/include)
include_directories(/home/eeros/pathos/Software/Colibri/pathoslib/include/)
link_directories(/home/eeros/efs/efs/build-arm/eeros/src/)
link_directories(/home/eeros/efs/efs/build-arm/flink/lib/)
link_directories(/home/eeros/pathos/Software/Colibri/pathoslib/build-arm)

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

# Executable, teaching
add_executable(peeping-panels-teach main_teach.cpp control/PeepingPanelControlSystem.cpp  
                                                   safety/PeepingPanelSafetyProperties_x4.cpp
                                                   sequences/MainSequence_teach.cpp
                                                   control/PathPlanner.cpp
                                                   sequences/Teaching.cpp       )   
target_link_libraries(peeping-panels-teach eeros pathos)

# Encoder test
add_executable(test-enc test/encoder/testEncoder.cpp)
target_link_libraries(test-enc eeros pathos)
