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
	double encPos_al = controlSys->getActualPos_rad(node_armLeft);
	controlSys->setPosRad_armLeft.setValue(encPos_al);   
	
	double encPos_ar = controlSys->getActualPos_rad(node_armRight);
	controlSys->setPosRad_armRight.setValue(encPos_ar);   
	
	sleep(1);
	double actualSetPoint_al = controlSys->setPosRad_armRight.getOut().getSignal().getValue();
	double actualSetPoint_ar = controlSys->setPosRad_armLeft.getOut().getSignal().getValue();
// 	std::cout << "after set " << actualSetPoint_al << "; " << actualSetPoint_ar << std::endl;
	
	double actPos_ar = encPos_ar;
	double actPos_al = encPos_al;
	bool positive_r = true;
	bool positive_l = true;
	
	while(/*controlSys->isOperationEnabled() && */!isTerminating()){		
		if( (actPos_ar <= 6.28) && positive_r){
			controlSys->setPosRad_armRight.setValue(actPos_ar);
			actPos_ar+=0.0628;
		}
		else{
			positive_r = false;
		}
		if( (actPos_ar >= -6.28) && !positive_r){
			controlSys->setPosRad_armRight.setValue(actPos_ar);
			actPos_ar-=0.0628;
		}
		else{
			positive_r = true;
		}
		
		if( (actPos_al <= 6.28) && positive_l){
			controlSys->setPosRad_armLeft.setValue(actPos_al);
			actPos_al+=0.0628;
		}
		else{
			positive_l = false;
		}
		if( (actPos_al >= -6.28) && !positive_l){
			controlSys->setPosRad_armLeft.setValue(actPos_al);
			actPos_al-=0.0628;
		}
		else{
			positive_l = true;
		}
		
		if(isTerminating()) break;
		usleep(100000);
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