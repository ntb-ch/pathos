#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_teach.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "../control/PeepingPanelControlSystem.hpp"
#include <eeros/core/EEROSException.hpp>
#include "../constants.hpp"
#include "../PeepingPanelConfig.hpp"

#include <unistd.h>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace pathos::peepingpanel;
using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;
// using namespace eeros::math;

MainSequence_teach::MainSequence_teach(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							teaching_s(sequencer, controlSys, safetySys) {
	// nothing to do
}

bool MainSequence_teach::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_teach::run() {
	log.trace() << "Sequencer '" << name << "': started.";

	while(safetySys->getCurrentLevel().getId() < ready) {    // power up & home
		usleep(100000); if (isTerminating()) return; }

	// Run teaching sequence
	safetySys->triggerEvent(doTeaching);  
	while(safetySys->getCurrentLevel().getId() < teaching) {    
		usleep(100000); if (isTerminating()) return; }
	teaching_s();    
	
}

void MainSequence_teach::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_teach::isTerminating() {
	return sequencer->getState() == state::terminating;
}