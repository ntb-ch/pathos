#include "Peep_in.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <unistd.h>
#include "../control/PeepingPanelControlSystem.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "../constants.hpp"

#include <iostream>

using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace eeros::safety;

Peep_in::Peep_in(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys) : 
		Sequence<>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// nothing to do
}

void Peep_in::init() {
	std::bind(&Peep_in::init, *this);
}

bool Peep_in::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() == ready;
}

void Peep_in::run() {
	log.info() << "[ Peeping Panel In ] started";
	
	AxisVector vel; vel = 30.0;
	controlSys->pathPlanner.setMaxSpeed(vel);
	
	AxisVector actual_pos = controlSys->sum_enc_offset.getOut().getSignal().getValue()(0) / i;
	AxisVector final_pos = ready_pos;
	AxisVector pos = final_pos - actual_pos;

	controlSys->pathPlanner.move(filename, time, pos);
	while (!controlSys->pathPlanner.posReached() && safetySys->getCurrentLevel().getId() == ready) {
		usleep(100000);
		if (isTerminating()) return;
		if (isStopping()) break;
	}
	sleep(0.5);
	
	// Back to ready position
	controlSys->pathPlanner.move(ready_pos, true);
	while (!controlSys->pathPlanner.posReached() && safetySys->getCurrentLevel().getId() == ready) {
			usleep(100000);
			if (isTerminating()) return;
			if (isStopping()) break;
	}
	sleep(0.5);
}

bool Peep_in::checkPostCondition() {
	return (safetySys->getCurrentLevel().getId() == ready); 
}

void Peep_in::exit() {
	log.info() << "[ Peeping Panel In ] exit done";
}

inline bool Peep_in::isTerminating() {
	return sequencer->getState() == state::terminating;
}

inline bool Peep_in::isStopping() {
// 	return !(safetySys->getCurrentLevel().getId() == goingToReady);
}

inline void Peep_in::setMotionCurve(std::string fn, double t) {
	filename = fn;
	
	if(t > 0.0)
		time = t;
	else
		log.warn() << "Wrong peeping time set, time must be positive";

}