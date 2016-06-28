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
	
	bool positive1 = true;
	bool positive2 = true;
	bool positive3 = true;
	bool positive4 = true;
	bool positive5 = true;
	
	auto actPos1 = controlSys->getActualPos_pulses(node1);
	auto actPos2 = controlSys->getActualPos_pulses(node2);
	auto actPos3 = controlSys->getActualPos_pulses(node3);
	auto actPos4 = controlSys->getActualPos_pulses(node4);
	auto actPos5 = controlSys->getActualPos_pulses(node5);
	int cnt = 0;
	
	while(!isTerminating()){
		// node1
		if( (actPos1 <= 10000) && positive1){
			controlSys->setPosPulses_node1.setValue(actPos1);
			actPos1+=100;
		}
		else{
			positive1 = false;
		}
		if( (actPos1 >= -10000) && !positive1){
			controlSys->setPosPulses_node1.setValue(actPos1);
			actPos1-=100;
		}
		else{
			positive1 = true;
		}
		// node2
		if( (actPos2 <= 10000) && positive2){
			controlSys->setPosPulses_node2.setValue(actPos2);
			actPos2+=100;
		}
		else{
			positive2 = false;
		}
		if( (actPos2 >= -10000) && !positive2){
			controlSys->setPosPulses_node2.setValue(actPos2);
			actPos2-=100;
		}
		else{
			positive2 = true;
		}
		// node3
		if( (actPos3 <= 10000) && positive3){
			controlSys->setPosPulses_node3.setValue(actPos3);
			actPos3+=100;
		}
		else{
			positive3 = false;
		}
		if( (actPos3 >= -10000) && !positive3){
			controlSys->setPosPulses_node3.setValue(actPos3);
			actPos3-=100;
		}
		else{
			positive3 = true;
		}
		// node 4
		if( (actPos4 <= 7000) && positive4){
			controlSys->setPosPulses_node4.setValue(actPos4);
			actPos4+=70;
		}
		else{
			positive4 = false;
		}
		if( (actPos4 >= -7000) && !positive4){
			controlSys->setPosPulses_node4.setValue(actPos4);
			actPos4-=70;
		}
		else{
			positive4 = true;
		}
		// node 5
		if( (actPos5 <= 7000) && positive5){
			controlSys->setPosPulses_node5.setValue(actPos5);
			actPos5+=70;
		}
		else{
			positive5 = false;
		}
		if( (actPos5 >= -7000) && !positive5){
			controlSys->setPosPulses_node5.setValue(actPos5);
			actPos5-=70;
		}
		else{
			positive5 = true;
		}

		if(isTerminating()) break;
		usleep(20000);
	}
	
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
