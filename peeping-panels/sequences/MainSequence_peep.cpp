#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_peep.hpp"
#include "../safety/SafetyProperties_Peep.hpp"
#include "../control/ControlSystem_Peep.hpp"
#include <unistd.h>
#include <iostream>

using namespace pathos::peepingpanel;
using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;

MainSequence_peep::MainSequence_peep(Sequencer* sequencer, ControlSystem_Peep* controlSys, SafetySystem* safetySys, std::array<double,3> configData) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							peep_out_s(sequencer, controlSys, safetySys), peep_in_s(sequencer, controlSys, safetySys) {
	
	peep_time = configData[0];
	
	if(configData[1] == -1.0)
		peep_direction = 'r'; 
	else //configData[1] == 1.0
		peep_direction =  'l'; 

	peep_angle     = configData[2];
}

bool MainSequence_peep::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_peep::run() {
	log.trace() << "[ Main Sequence Started ]";

	while(safetySys->getCurrentLevel().getId() < ready) {    // power up & home
		usleep(100000); if (isTerminating()) return; }
	
	// TEST behaviour
	
	// Init counters
	int sleep_count = 0;
	
	// Start state machine
	int status_panel = 1;
	int count = 0;
	while(count < 20) {  // TODO change condition of while loop
		switch(status_panel) {
			case 1:                              // In
				log.info() << "in";
				
// 				// Check if needs to go to sleep
// 				if(sleep_count > 3){
// 					status_panel = 6;
// 					sleep_count = 0;
// 				}
// 				else {
// 					sleep_count = sleep_count + 1;
// 					sleep(1);
// 				}
// 				std::cout << "sleep_count: " << sleep_count << std::endl;
				
				// Check if it's time to peep (sensors) -> to be combined with if sleep
				
				// Example of check sensors data
// 				controlSys->sensorsInputData.getOutStatus().getSignal().getValue();
				
				sleep(1);
				
				// Go to 'peep out' state
				status_panel = 2;
				break;
				
			case 2:                              // Peeping out
				peep_out_s.setMotionCurve("/mnt/data/curves/curve_input1.txt", peep_time, peep_angle, peep_direction);
				peep_out_s();
				log.info() << "Peep out";
				sleep(3);
				
				// Go to 'out' state
				status_panel = 3;
				break;
				
			case 3:                              // Out
				log.info() << "Out"; 
				// wait a bit (time dependent from sensors data)
				sleep(2);
				
				// Go to 'peep in' state
				status_panel = 4;
				break;
				
			case 4:                               // Peeping in
				log.info() << "Peeping in"; 
				
				// Peep in 
				peep_in_s.setMotionCurve("/mnt/data/curves/curve_input1.txt" , peep_time);
				peep_in_s();
				
				// Go to 'in' state
				status_panel = 1;
				break;
				
			case 5:                               // Sudden stop (maybe not needed)
				log.info() << "Sudden stop"; 
				sleep(3);
				break;
				
			case 6:                               // Sleep
				log.info() << "Sleep";
				
				// Go to 'sleep' status in safety (diable drives)
				safetySys->triggerEvent(doSleep);
				
				// Sleeps until one sensor detects one person in the room
				sleep(3);
				
				// Go to 'awake' status
				status_panel = 7;
				break;
				
			case 7:                               // Awake
				log.info() << "Awake"; 
				
				// Do awake safety system
				safetySys->triggerEvent(doAwake);
				while(safetySys->getCurrentLevel().getId() < ready) {   
					usleep(100000); if (isTerminating()) return; }
				
				// Back to ready position
				controlSys->pathPlanner.move(ready_pos, true);
				while (!controlSys->pathPlanner.posReached() && safetySys->getCurrentLevel().getId() == ready) {
					usleep(100000);
					if (isTerminating()) return;
				}
				
				// Go to 'in' status
				status_panel = 1;
				break;
				
			default : // TODO
				break;
		}
		count++;
	}

}

void MainSequence_peep::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_peep::isTerminating() {
	return sequencer->getState() == state::terminating;
}
