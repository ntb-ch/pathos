#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_decoy.hpp"
#include "../control/ControlSystem_decoy.hpp"
#include <unistd.h>
#include <iostream>
#include <canopen-faulhaber-drv.h>

#include "../constants.hpp"

using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace pathos::decoy;

MainSequence_decoy::MainSequence_decoy(Sequencer* sequencer, ControlSystem_decoy* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							homing_s(sequencer, controlSys, safetySys), armMotion_s(sequencer, controlSys, safetySys) {
	// do something...
}

bool MainSequence_decoy::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_decoy::run() {
// 	log.info() << "[ Main Sequence Started ]"; // TODO understand why error
	
	safetySys->triggerEvent(doEnable);
	
	// 1. Wait for robot initialization + set motion parameters
	while(safetySys->getCurrentLevel().getId() < homed){
		if(isTerminating()) break;
		usleep(10000);
	}
	auto pos_left  = controlSys->getActualPos_pulses(node_armLeft) ;
	auto pos_right = controlSys->getActualPos_pulses(node_armRight);
	std::cout << "main sequence homed: " << pos_left << "; " << pos_right << std::endl;
	
	while(safetySys->getCurrentLevel().getId() < ready){
		if(isTerminating()) break;
		usleep(10000);
	}
	
	/*auto*/ pos_left  = controlSys->getActualPos_pulses(node_armLeft) ;
	/*auto*/ pos_right = controlSys->getActualPos_pulses(node_armRight);
	std::cout << "main sequence ready: " << pos_left << "; " << pos_right << std::endl;
	
	// 3. Perform motion
// 	armMotion_s();

}

void MainSequence_decoy::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_decoy::isTerminating() {
	return sequencer->getState() == state::terminating;
}
