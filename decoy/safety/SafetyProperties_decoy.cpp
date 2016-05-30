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

#include "../initMotors.hpp"

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
		{homing,             "Homing motors",             },
		{homed,              "Motors homed",              },
		{settingMotionParam, "Setting motion parameters", },
		{ready,              "Motors ready ",             }
	};
	
	// Add events to the levels
	level(off                ).addEvent(doEnable,       initMotors,         kPublicEvent  );
	level(initMotors         ).addEvent(initMotorsDone, motorsEnabling,     kPrivateEvent );
	level(motorsEnabling     ).addEvent(enablingDone,   motorsEnabled,      kPrivateEvent );
	level(motorsEnabled      ).addEvent(doDisable,      off,                kPrivateEvent );
	level(motorsEnabled      ).addEvent(doHome,         homing,             kPrivateEvent );
	level(homing             ).addEvent(homingDone,     homed,              kPublicEvent  );
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
			if(controlSys->isOperationEnabled()){
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
	
	level(homing).setLevelAction([this](SafetyContext* privateContext) {
		homing_motors_actualPos(socket);
		homing_motors_endSwitch(socket);
		privateContext->triggerEvent(homingDone);
	});
	
	level(homed).setLevelAction([this](SafetyContext* privateContext) {
		privateContext->triggerEvent(setMotionParam);
	});
	level(settingMotionParam).setLevelAction([this](SafetyContext* privateContext) {
		set_motion_parameters(socket);
		privateContext->triggerEvent(doReady);
	});
	
	
	// Define entry level
	entryLevel = off;
}

SafetyProperties_decoy::~SafetyProperties_decoy() {
	// nothing to do
}