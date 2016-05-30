#include <eeros/safety/SafetySystem.hpp>
#include "CanPathMainSequence.hpp"
#include "../control/CanControlSystem.hpp"
#include <unistd.h>
#include <iostream>

using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;

CanPathMainSequence::CanPathMainSequence(Sequencer* sequencer, CanControlSystem* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// do something...
}

bool CanPathMainSequence::checkPreCondition() {
	return true;
}

void CanPathMainSequence::run() {
	log.trace() << "[ Main Sequence Started ]";

	controlSys->setPos.setValue(0);
	usleep(10000);
	controlSys->setPos.setValue(1000);
	usleep(10000);
	controlSys->setPos.setValue(-1000);
	
	int cnt = 0;
	int16_t drvCtrl = 0;
	int32_t data = 0;
	bool ready = false;
	int32_t actPos = 0;
	bool pos = true;
	
	log.info() << "starting...";
	
	while(!isTerminating()){
		cnt++;
		if(ready && (cnt % 1 == 0)){
			cnt = 0;
			if( (actPos <= 50000) && pos){
				controlSys->setPos.setValue(actPos);
				actPos+=500;
			}
			else{
				pos = false;
			}
			if( (actPos >= -50000) && !pos){
				controlSys->setPos.setValue(actPos);
				actPos-=500;
			}
			else{
				pos = true;
			}
		}
		if(ready){
			controlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO2_TX);
			if(controlSys->canReceive.getPdoValue(0x05, CANOPEN_FC_PDO2_TX, &drvCtrl, &data)!=0){
				log.info() << "function code not found";
			}
			else{
				std::cout << std::hex << "ctrl: " << drvCtrl << std::dec << "\t data: " << data << std::endl; 
			}
		}
		else {
			controlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO1_TX);
			if(controlSys->canReceive.getPdoValue(0x05, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){
				log.info() << "function code not found";
			}
			else{
				if( (drvCtrl & 0x002F) == 0x0027 ){
					ready = true;
					std::cout << std::hex << "status: " << drvCtrl << std::endl; 
				}
				else{
					std::cout << std::hex << "status: " << drvCtrl << std::endl; 
				}
			}
		}
		usleep(10000);
	}

}

void CanPathMainSequence::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool CanPathMainSequence::isTerminating() {
	return sequencer->getState() == state::terminating;
}