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
							armMotion_s(sequencer, controlSys, safetySys) {
	// do something...
}

bool MainSequence_decoy::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_decoy::run() {
	log.info() << "[ Main Sequence Started ]";
	
	safetySys->triggerEvent(doEnable);
	
	// 1. Wait that robot is ready
	while(safetySys->getCurrentLevel().getId() < ready){
		if(isTerminating()) break;
		usleep(10000);
	}
	
	// 2. Perform motion
	armMotion_s();

}

void MainSequence_decoy::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_decoy::isTerminating() {
	return sequencer->getState() == state::terminating;
}
