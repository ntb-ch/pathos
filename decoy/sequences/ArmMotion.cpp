#include "../control/ControlSystem_decoy.hpp"
#include "../safety/SafetyProperties_decoy.hpp"
#include "ArmMotion.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <unistd.h>
#include <iostream>
#include "../constants.hpp"

using namespace pathos::decoy;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace eeros::safety;

ArmMotion::ArmMotion(Sequencer* sequencer, ControlSystem_decoy* controlSys, SafetySystem* safetySys) : 
		Sequence<>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// nothing to do
}

void ArmMotion::init() {
	std::bind(&ArmMotion::init, *this);
}

bool ArmMotion::checkPreCondition() {
	return true;
}

void ArmMotion::run() {
	log.info() << "[ Arm Motion ] started";
	
	// Set ref position to actual position
	double encPos = controlSys->getActualPos_rad();
	controlSys->setPosRad.setValue(encPos);   
	
	sleep(1);
	double actualSetPoint = controlSys->setPosRad.getOut().getSignal().getValue();
	std::cout << "after set " << actualSetPoint << std::endl;
	
// 	std::cout << "encPos: " << encPos << "; setPoint: " << controlSys->setPosRad.getOut().getSignal().getValue() << std::endl;
	
	double actPos = encPos;
	bool positive = true;
	
	while(controlSys->isOperationEnabled() && !isTerminating()){		
		if( (actPos <= 6.28) && positive){
			controlSys->setPosRad.setValue(actPos);
			actPos+=0.0628;
		}
		else{
			positive = false;
		}
		if( (actPos >= -6.28) && !positive){
			controlSys->setPosRad.setValue(actPos);
			actPos-=0.0628;
		}
		else{
			positive = true;
		}
		usleep(10000);
	}
	sleep(1);
}

bool ArmMotion::checkPostCondition() {
	return true;
}

void ArmMotion::exit() {
	log.info() << "[ ArmMotion ] exit done";
}

inline bool ArmMotion::isTerminating() {
	return sequencer->getState() == state::terminating;
}