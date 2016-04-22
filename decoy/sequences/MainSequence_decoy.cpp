#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_decoy.hpp"
#include "../control/ControlSystem_decoy.hpp"
#include <unistd.h>
#include <iostream>

#include "../constants.hpp"

using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace pathos::decoy;

MainSequence_decoy::MainSequence_decoy(Sequencer* sequencer, ControlSystem_decoy* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							homing(sequencer, controlSys, safetySys), armMotion(sequencer, controlSys, safetySys) {
	// do something...
}

bool MainSequence_decoy::checkPreCondition() {
	return true;
}

void MainSequence_decoy::run() {
	log.trace() << "[ Main Sequence Started ]";
	
	controlSys->setPosRad.setValue(0.001);   // TODO, why needed?
	
	// 1. Wait for "operation enabled" 
	while(safetySys->getCurrentLevel().getId() != motorsEnabled)
		usleep(10000);

	// 2. Homing
	homing();
	
	// 3. Perform motion
	armMotion();

}

void MainSequence_decoy::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_decoy::isTerminating() {
	return sequencer->getState() == state::terminating;
}