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
	
	bool positive4 = true;
	bool positive5 = true;
	
	auto actPos4 = controlSys->getActualPos_pulses(node4);
	auto actPos5 = controlSys->getActualPos_pulses(node5);
	int cnt = 0;
	
	while(!isTerminating()){
// 		log.info() << "set pos";
// 		controlSys->setPosRad_node4.setValue(5.0);
// 		controlSys->setPosRad_node5.setValue(0.0);
// 		
// 		sleep(1);
// 		
// 		auto o1 = controlSys->setPosRad_node4.getOut().getSignal().getValue();
// 		auto o2 = controlSys->setPosRad_node5.getOut().getSignal().getValue();
// 		std::cout << " oooo: " <<  o1 << "; " << o2 << std::endl;
// 		
// 		while(!isTerminating()){
// 			
// 			auto ref4 = controlSys->setPosRad_node4.getOut().getSignal().getValue();
// 			auto ref4p = controlSys->radToPulses_node4.getOut().getSignal().getValue();
// 			auto enc4p = controlSys->getActualPos_pulses(node4);
// 			auto enc4 = controlSys->getActualPos_rad(node4);
// 			auto sw4 = controlSys->getStatusWord(node4);
// 
// 			auto ref5 = controlSys->setPosRad_node5.getOut().getSignal().getValue();
// 			auto ref5p = controlSys->radToPulses_node5.getOut().getSignal().getValue();
// 			auto enc5p = controlSys->getActualPos_pulses(node5);
// 			auto enc5 = controlSys->getActualPos_rad(node5);
// 			auto sw5 = controlSys->getStatusWord(node5);
// 			
// 			std::cout << std::hex << sw4 << ";\t" << std::dec << ref4 << ";\t" << ref4p << ";\t" << enc4p << ";\t" << enc4 << " //\t" 
// 			          << std::hex << sw5 << ";\t" << std::dec << ref5 << ";\t" << ref5p << ";\t" << enc5p << ";\t" << enc5 << std::endl;
// 					  
// // 			std::cout << "spr: " << controlSys->setPointReceived(node4) << "; " << controlSys->setPointReached(node4) << std::endl;
// 			usleep(1000000);
// 		}
		
// 		controlSys->setPosPulses_node4.setValue(40000);
// 		sleep(2);
// 		controlSys->getActualPos_pulses(node4);
// 		controlSys->canSend.sendPdo(0x04, CANOPEN_FC_PDO1_RX, 0x000F);
// 		sleep(2);
// 		controlSys->canSend.initiatePdoRequest(0x04, CANOPEN_FC_PDO2_TX);
// 		std::cout << "data: " << controlSys->canReceive.getUserDataOut(0x04).getSignal().getValue() << std::endl;
// 		
// 		controlSys->setPosPulses_node4.setValue(-40000);
// 		sleep(2);
// 		controlSys->getActualPos_pulses(node4);
// 		controlSys->canSend.sendPdo(0x04, CANOPEN_FC_PDO1_RX, 0x000F);
// 		sleep(2);
// 		controlSys->canSend.initiatePdoRequest(0x04, CANOPEN_FC_PDO2_TX);
// 		std::cout << "data: " << controlSys->canReceive.getUserDataOut(0x04).getSignal().getValue() << std::endl;
		
		
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
		
		auto ref4p = controlSys->setPosPulses_node4.getOut().getSignal().getValue();
// 		auto enc4p = controlSys->getActualPos_pulses(node4);
		auto enc4p = controlSys->canReceive.getUserDataOut(node4).getSignal().getValue();
		auto sw4 = controlSys->canReceive.getdrvCtrlOut(node4).getSignal().getValue();
		
		auto ref5p = controlSys->setPosPulses_node5.getOut().getSignal().getValue();
		auto enc5p = controlSys->canReceive.getUserDataOut(node5).getSignal().getValue();
		auto sw5 = controlSys->canReceive.getdrvCtrlOut(node5).getSignal().getValue();
		
		controlSys->canSend.initiatePdoRequest(node4, CANOPEN_FC_PDO2_TX);
		controlSys->canSend.initiatePdoRequest(node5, CANOPEN_FC_PDO2_TX);
// 		std::cout << "data: " << controlSys->canReceive.getUserDataOut(0x04).getSignal().getValue() << std::endl;
		cnt++;
		if(cnt > 10){
			std::cout << "4:\t" << ref4p << ";\t" << std::hex << sw4 << ";\t" << std::dec << enc4p << std::endl;
			std::cout << "5:\t" << ref5p << ";\t" << std::hex << sw5 << ";\t" << std::dec << enc5p << std::endl;
			cnt = 0;
		}
// 		std::cout << std::hex << sw4 << ";\t" << std::dec << actPos4 << ";\t" << ref4p << ";\t" << enc4p << std::endl; 
 
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