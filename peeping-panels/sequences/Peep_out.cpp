#include "Peep_out.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <unistd.h>
#include "../control/ControlSystem_Peep.hpp"
#include "../safety/SafetyProperties_Peep.hpp"
#include "../constants.hpp"

#include <iostream>

using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace eeros::safety;

Peep_out::Peep_out(Sequencer* sequencer, ControlSystem_Peep* controlSys, SafetySystem* safetySys) : 
		Sequence<>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// nothing to do
}

void Peep_out::init() {
	std::bind(&Peep_out::init, *this);
}

bool Peep_out::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() == ready;
}

void Peep_out::run() {
	log.info() << "[ Peeping Panel out ] started";
	
	AxisVector vel; vel = 30.0;
	controlSys->pathPlanner.setMaxSpeed(vel);
	
	AxisVector actual_pos = controlSys->sum_enc_offset.getOut().getSignal().getValue()(0) / i_gear;
	AxisVector final_pos = peepDirection * peepAngle;
	AxisVector pos = final_pos - actual_pos;

	bool reset = false;
	int count = 0;
	controlSys->pathPlanner.move(filename, time, pos);
	while (!controlSys->pathPlanner.posReached() && safetySys->getCurrentLevel().getId() == ready) {
		usleep(100000);
		if (isTerminating()) return;
		if (isStopping()) break;
// 		count = count +1;
// 		if(count == 5){
// 			log.info() << "reset";
// 			controlSys->pathPlanner.reset();
// 			reset = true;
// 			break;
// 		}
	}
	if(reset){
		// Back to ready position
		controlSys->pathPlanner.move(ready_pos, true);
		while (!controlSys->pathPlanner.posReached() && safetySys->getCurrentLevel().getId() == ready) {
			usleep(100000);
			if (isTerminating()) return;
			if (isStopping()) break;
		}
	}
// 	sleep(0.5);
}

bool Peep_out::checkPostCondition() {
	return (safetySys->getCurrentLevel().getId() == ready); 
}

void Peep_out::exit() {
	log.info() << "[ Peeping Panel out ] exit done";
}

inline bool Peep_out::isTerminating() {
	return sequencer->getState() == state::terminating;
}

inline bool Peep_out::isStopping() {
// 	return controlSys->sensorsData.getOut().getSignal()
}

inline void Peep_out::setMotionCurve(std::string fn, double t, double angle, char direction) {
	filename = fn;
	
	if(t > 0.0)
		time = t;
	else
		log.warn() << "Wrong peeping time set, time must be positive";
	
	if(angle >= -0.087 && angle <= 1.60)
		peepAngle = angle;
	else
		log.warn() << "Wrong peeping angle set (enter angle in rad between -0.087 and 1.60)";
	
	if(direction == 'r')
		peepDirection = -1.0; 
	else if(direction == 'l')
		peepDirection =  1.0; 
	else
		log.warn() << "Wrong peeping direction set ('r' or 'l')";
}

// inline void Peep_out::getSensorsActions() {
// 	sensorData = controlSys->getSensorData();
/*	
	if (sensorData == visible) -> suddenly interrupt && go To in
	else if (sensorData == far) -> suddenly interrupt and k
	
	
}*/