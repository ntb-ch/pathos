#include "SafetyProperties_decoy.hpp"
#include "../control/ControlSystem_decoy.hpp"
#include "../constants.hpp"
#include <eeros/hal/HAL.hpp>
#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/inputActions.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/safety/ControlInput.hpp>
#include <eeros/math/Matrix.hpp>

#include <unistd.h>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <cmath>

#include "../motorsFunctions.hpp"

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;
using namespace pathos::decoy;

SafetyProperties_decoy::SafetyProperties_decoy(int canSock, ControlSystem_decoy* cs) : socket(canSock), controlSys(cs) {

	HAL& hal = HAL::instance();
	
	// Define Levels
	levels = {
		{ off,               "System off",                },
		{initMotors,         "Init motors",               },
		{motorsEnabling,     "Enabling motors",           },
		{motorsEnabled,      "Motors enabled",            },
		{homing1,            "Homing motor 1",            },
		{homing2,            "Homing motor 2",            },
		{homing3,            "Homing motor 3",            },
		{homing4,            "Homing motor 4",            },
		{homing5,            "Homing motor 5",            },
		{homed,              "Motors homed",              },
		{settingMotionParam, "Setting motion parameters", },
		{ready,              "Motors ready ",             }
	};
	
	// Add events to the levels
	level(off                ).addEvent(doEnable,       initMotors,         kPublicEvent  );
	level(initMotors         ).addEvent(initMotorsDone, motorsEnabling,     kPrivateEvent );
	level(motorsEnabling     ).addEvent(enablingDone,   motorsEnabled,      kPrivateEvent );
	level(motorsEnabled      ).addEvent(doDisable,      off,                kPrivateEvent );
	level(motorsEnabled      ).addEvent(doHome,         homing1,            kPrivateEvent );
	level(homing1            ).addEvent(homingDone1,    homing2,            kPublicEvent  );
	level(homing2            ).addEvent(homingDone2,    homing3,            kPublicEvent  );
	level(homing3            ).addEvent(homingDone3,    homing4,            kPublicEvent  );
	level(homing4            ).addEvent(homingDone4,    homing5,            kPublicEvent  );
	level(homing5            ).addEvent(homingDone5,    homed,              kPublicEvent  );                                                        
	level(homed              ).addEvent(setMotionParam, settingMotionParam, kPublicEvent  );
	level(settingMotionParam ).addEvent(doReady,        ready,              kPublicEvent  );

	// *** Define and add level functions *** //
	
	level(initMotors).setLevelAction([this](SafetyContext* privateContext) {
		init_motors(socket);
		controlSys->canSend.enable();
		controlSys->canReceive.enable();
		privateContext->triggerEvent(initMotorsDone);
	});
	
	level(motorsEnabling).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		static int timeout = 0;
		
		if(first == true && count > 200 ) {
// 			if(controlSys->isOperationEnabled()){
			if(controlSys->canReceive.getOpEnabledOut().getSignal().getValue()){
				first = false;
				privateContext->triggerEvent(enablingDone);
			}
			count = 0;
			timeout = timeout + 2.0;
		}
		else {
			count++;
		}
		if(timeout > 20)
			throw eeros::EEROSException("Drives enabling process timed out");
	});
	
	level(motorsEnabled).setLevelAction([this](SafetyContext* privateContext) {
		privateContext->triggerEvent(doHome);
	});
	
	level(homing1).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		
		if(first){
			homing_motor(socket, node1, homingMtd[0]);
			first = false;
		}
		count++;
		
		if(controlSys->isHomed(node1) && count > 100){
			privateContext->triggerEvent(homingDone1);
		}
	});
	level(homing2).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		
		if(first){
			homing_motor(socket, node2, homingMtd[1]);
			first = false;
		}
		count++;
		
		if(controlSys->isHomed(node2) && count > 100){
			privateContext->triggerEvent(homingDone2);
		}
	});
	level(homing3).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		
		if(first){
			homing_motor(socket, node3, homingMtd[2]);
			first = false;
		}
		count++;
		
		if(controlSys->isHomed(node3) && count > 100){
			privateContext->triggerEvent(homingDone3);
		}
	});
	level(homing4).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		
		if(first){
			homing_motor(socket, node4, homingMtd[3]); 
			first = false;
		}
		count++;
		
		if(controlSys->isHomed(node4) && count > 100){
			privateContext->triggerEvent(homingDone4);
		}
	});

	level(homing5).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static int count = 0;
		
		if(first){
			homing_motor(socket, node5, homingMtd[4]); 
			first = false;
		}
		count++;
		
		if(controlSys->isHomed(node5) && count > 100){
			privateContext->triggerEvent(homingDone5);
		}
	});
	
	level(homed).setLevelAction([this](SafetyContext* privateContext) {
		auto n1 = controlSys->getActualPos_pulses(node1);
		auto n2 = controlSys->getActualPos_pulses(node2);
		auto n3 = controlSys->getActualPos_pulses(node3);
		auto n4 = controlSys->getActualPos_pulses(node4);
		auto n5 = controlSys->getActualPos_pulses(node5);
		
		std::cout << n1 << "; " << n2 << "; " << n3 << "; " << n4 << "; " << n5 << std::endl;
		
		privateContext->triggerEvent(setMotionParam);
	});
	
	level(settingMotionParam).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		
		if(first){
			set_position_profile_mode(socket);
			set_motion_parameters(socket);
			
			double encPos_al = controlSys->getActualPos_rad(node4);
			double encPos_ar = controlSys->getActualPos_rad(node5);
			
			first = false;
		}
		
		// Do ready if drives in operation enabled (bits 10 and 12 resetted)
		if(controlSys->canReceive.getOpEnabledOut().getSignal().getValue()){
// 		if(controlSys->isOperationEnabled()) // TODO leave or change?
			int j = 0;
			for( j = 0; j < sizeof(nodes)/sizeof(nodes[0]); j++){
					
				if(!controlSys->setPointReceived(nodes[j])){
					privateContext->triggerEvent(doReady);
					std::cout << "byebye" << std::endl;
				}
// 				else{ // TODO FAILED to initiate PDO request !!! ???
// 					controlSys->canSend.sendPdo(nodes[j], CANOPEN_FC_PDO1_RX, 0x000F);
// 				}
			} 
		}
	});
	
// 	level(ready).setLevelAction([this](SafetyContext* privateContext) {
// 		static int count = 0;
// 		int statusWords[5];
// 		
// 		if(count > 100){
// 			for (int j=0;j<5;j++)
// 				statusWords[j] = controlSys->getStatusWord(nodes[j]);
// 			count = 0;
// 			std::cout << "sw: " << statusWords[0] << "; " << 
// 			                       statusWords[1] << "; " << 
// 			                       statusWords[2] << "; " << 
// 			                       statusWords[3] << "; " << 
// 			                       statusWords[4] << "; " << std::endl;
// 		}
// 		else count++;
// 	});
	
	// Define entry level
	entryLevel = off;
}

SafetyProperties_decoy::~SafetyProperties_decoy() {
	// nothing to do
}