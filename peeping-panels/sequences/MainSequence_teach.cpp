#include "MainSequence_teach.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include "../safety/SafetyProperties_Teach.hpp"
#include "../control/ControlSystem_Teach.hpp"
#include <unistd.h>

using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace pathos::peepingpanel;

MainSequence_teach::MainSequence_teach(Sequencer* sequencer, ControlSystem_Teach* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							teaching_s(sequencer, controlSys, safetySys) {
	// nothing to do
}

bool MainSequence_teach::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_teach::run() {
	log.trace() << "[ Main Sequence Started ]";

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