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
		if( (actPos2 <= 5000) && positive2){
			controlSys->setPosPulses_node2.setValue(actPos2);
			actPos2+=100;
		}
		else{
			positive2 = false;
		}
		if( (actPos2 >= -5000) && !positive2){
			controlSys->setPosPulses_node2.setValue(actPos2);
			actPos2-=100;
		}
		else{
			positive2 = true;
		}
		// node3
		if( (actPos3 <= 5000) && positive3){
			controlSys->setPosPulses_node3.setValue(actPos3);
			actPos3+=100;
		}
		else{
			positive3 = false;
		}
		if( (actPos3 >= -5000) && !positive3){
			controlSys->setPosPulses_node3.setValue(actPos3);
			actPos3-=100;
		}
		else{
			positive3 = true;
		}
		// node 4
		if( (actPos4 <= 40000/*6.28*/) && positive4){
			controlSys->setPosPulses_node4.setValue(actPos4);
			actPos4+=400;
		}
		else{
			positive4 = false;
		}
		if( (actPos4 >= -40000) && !positive4){
			controlSys->setPosPulses_node4.setValue(actPos4);
			actPos4-=400;
		}
		else{
			positive4 = true;
		}
		// node 5
		if( (actPos5 <= 20000/*6.28*/) && positive5){
			controlSys->setPosPulses_node5.setValue(actPos5);
			actPos5+=400;
		}
		else{
			positive5 = false;
		}
		if( (actPos5 >= -20000) && !positive5){
			controlSys->setPosPulses_node5.setValue(actPos5);
			actPos5-=400;
		}
		else{
			positive5 = true;
		}
		
// // // 		auto ref4p = controlSys->setPosPulses_node4.getOut().getSignal().getValue();
// // // 		auto enc4p = controlSys->canReceive.getUserDataOut(node4).getSignal().getValue();
// // // 		auto sw4 = controlSys->canReceive.getdrvCtrlOut(node4).getSignal().getValue();
// // // 		
// // // 		auto ref5p = controlSys->setPosPulses_node5.getOut().getSignal().getValue();
// // // 		auto enc5p = controlSys->canReceive.getUserDataOut(node5).getSignal().getValue();
// // // 		auto sw5 = controlSys->canReceive.getdrvCtrlOut(node5).getSignal().getValue();
		
// 		controlSys->canSend.initiatePdoRequest(node1, CANOPEN_FC_PDO2_TX); // TODO needed?
// 		controlSys->canSend.initiatePdoRequest(node2, CANOPEN_FC_PDO2_TX); // TODO needed?
// 		controlSys->canSend.initiatePdoRequest(node3, CANOPEN_FC_PDO2_TX); // TODO needed?
// 		controlSys->canSend.initiatePdoRequest(node4, CANOPEN_FC_PDO2_TX); // TODO needed?
// 		controlSys->canSend.initiatePdoRequest(node5, CANOPEN_FC_PDO2_TX); // TODO needed?

// // // // 		std::cout << "data: " << controlSys->canReceive.getUserDataOut(0x04).getSignal().getValue() << std::endl;
// // // 		cnt++;
// // // 		if(cnt > 10){
// // // 			std::cout << "2:\t" << ref2p << ";\t" << std::hex << sw2 << ";\t" << std::dec << enc2p << std::endl;
// // // 			std::cout << "3:\t" << ref3p << ";\t" << std::hex << sw3 << ";\t" << std::dec << enc3p << std::endl;
// // // 			cnt = 0;
// // // 		}
// // // // 		std::cout << std::hex << sw4 << ";\t" << std::dec << actPos4 << ";\t" << ref4p << ";\t" << enc4p << std::endl; 
 
		if(isTerminating()) break;
		usleep(20000);
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
