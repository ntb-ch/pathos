#include "../control/ControlSystem_decoy.hpp"
#include "../safety/SafetyProperties_decoy.hpp"
#include "Homing.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <unistd.h>
#include <iostream>
#include "../constants.hpp"

using namespace pathos::decoy;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace eeros::safety;

Homing::Homing(Sequencer* sequencer, ControlSystem_decoy* controlSys, SafetySystem* safetySys) : 
		Sequence<>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// nothing to do
}

void Homing::init() {
	std::bind(&Homing::init, *this);
}

bool Homing::checkPreCondition() {
	return true;
}

void Homing::run() {
	log.info() << "[ Homing ] started";
	
	double encPos = controlSys->getActualPos_rad();
	double targetPos = 0.0;
	
	// Set ref position to actual position
	controlSys->setPosRad.setValue(encPos);   
	usleep(100000);
	
	double setPos = encPos;
	while(controlSys->isOperationEnabled() && !isTerminating()){
		if(fabs(setPos - targetPos) < 0.01){
			break;
		}
		else if(setPos<targetPos){
			controlSys->setPosRad.setValue(setPos);
			setPos += 0.005;
		}
		else if(setPos>targetPos){
			controlSys->setPosRad.setValue(setPos);
			setPos -= 0.005;
		}
		usleep(10000);
	}
	
	// Ref = enc --> TODO delete after solving probel of encoder not reaching setpoint
	encPos = controlSys->getActualPos_rad();
	controlSys->setPosRad.setValue(encPos);
	usleep(100000);
	std::cout << "setPoint: " << controlSys->setPosRad.getOut().getSignal().getValue() << "; encPos: " << encPos << std::endl;
	
}

bool Homing::checkPostCondition() {
	return true;
}

void Homing::exit() {
	log.info() << "[ Homing ] exit done";
}

inline bool Homing::isTerminating() {
	return sequencer->getState() == state::terminating;
}