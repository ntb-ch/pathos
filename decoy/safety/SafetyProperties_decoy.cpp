#include "SafetyProperties_decoy.hpp"
#include "../control/ControlSystem_decoy.hpp"
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

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;
using namespace pathos::decoy;

SafetyProperties_decoy::SafetyProperties_decoy(ControlSystem_decoy* cs) : controlSys(cs) {

	HAL& hal = HAL::instance();

	// Define critical outputs
	
	// Define critical inputs
	
	// Define Levels
	levels = {
		{ off,          "System off",        },
		{motorsEnabled, "Motors are active", }
	};
	
	// Add events to the levels
	level(off).addEvent(doEnable, motorsEnabled, kPrivateEvent);
	
	// Set input actions
	level(off          ).setInputActions({ });
	level(motorsEnabled).setInputActions({ });
	
	// Set output actions
	level(off          ).setOutputActions({ });  
	level(motorsEnabled).setOutputActions({ });  
	
	// *** Define and add level functions *** //

	level(off).setLevelAction([this](SafetyContext* privateContext) {
		if(controlSys->isOperationEnabled())
			privateContext->triggerEvent(doEnable);
	});
	
	// Define entry level
	entryLevel = off;
}

SafetyProperties_decoy::~SafetyProperties_decoy() {
	// nothing to do
}