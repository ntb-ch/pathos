#include "CanSafetyProperties.hpp"
#include "../control/CanControlSystem.hpp"
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

CanSafetyProperties::CanSafetyProperties(CanControlSystem* cs) : controlSys(cs) {

	HAL& hal = HAL::instance();

	// ############ Define critical outputs ############
	
	// ############ Define critical inputs ############
	
	// ############ Define Levels ############
	levels = {
		{ off,                "System off",                  }
	};
	
	// ############ Add events to the levels ############
	
	// ############ Define input states and events for all levels ############
		
	// Define output states and events for all levels 

	// *** Define and add level functions *** //

	// Define entry level
	entryLevel = off;
}

CanSafetyProperties::~CanSafetyProperties() {
	// nothing to do
}