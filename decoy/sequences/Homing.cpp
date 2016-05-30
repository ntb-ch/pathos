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
	return (safetySys->getCurrentLevel().getId() == homing);
}

void Homing::run() {
	log.info() << "[ Homing ] started";
	
	auto pos_left = controlSys->getActualPos_rad(node_armLeft);
	auto pos_right = controlSys->getActualPos_rad(node_armRight);
	
	log.info() << pos_left << "; " << pos_right;
	
// 	controlSys->homingMotors();

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