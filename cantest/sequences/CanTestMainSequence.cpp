#include <eeros/safety/SafetySystem.hpp>
#include "CanTestMainSequence.hpp"
#include "../control/CanControlSystem.hpp"
#include <unistd.h>
#include <iostream>

using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;

CanTestMainSequence::CanTestMainSequence(Sequencer* sequencer, CanControlSystem* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// do something...
}

bool CanTestMainSequence::checkPreCondition() {
	return true;
}

void CanTestMainSequence::run() {
	log.trace() << "[ Main Sequence Started ]";

	controlSys->setPos.setValue(0);
	usleep(10000);
	controlSys->setPos.setValue(1000);
	usleep(10000);
	controlSys->setPos.setValue(-1000);
	
	int cnt = 0;
	bool neg = false;
	int16_t drvCtrl = 0;
	int32_t data = 0;
	bool ready = false;
	
	log.info() << "Starting...";
	
	while(!isTerminating()){
		cnt++;
		if(ready && (cnt % 400 == 0)){
			cnt = 0;
			if(neg){
				controlSys->setPos.setValue(16000);
				neg = false;
			}
			else{
				controlSys->setPos.setValue(-16000);
				neg = true;
			}
			controlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO2_TX);
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
				std::cout << ready << "; " << std::hex << drvCtrl << std::endl;   // 60, 33
				if( (drvCtrl & 0x002F) == 0x0027 ){                               // 0x0027 = 39
					log.info() << ready ; 
					ready = true;
// 					std::cout << std::hex << "status: " << drvCtrl << std::endl; 
				}
				else{
// 					std::cout << std::hex << "status: " << drvCtrl << std::endl;  // hier 
				}
			}
		}
		usleep(10000);
	}

}

void CanTestMainSequence::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool CanTestMainSequence::isTerminating() {
	return sequencer->getState() == state::terminating;
}