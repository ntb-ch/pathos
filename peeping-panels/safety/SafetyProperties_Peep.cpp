#include "SafetyProperties_Peep.hpp"
#include "../control/ControlSystem_Peep.hpp"

#include <eeros/hal/HAL.hpp>
#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/inputActions.hpp>
#include <eeros/safety/OutputAction.hpp>

#include <unistd.h>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <cmath>
#include <algorithm>

#include "../constants.hpp"

using namespace pathos::peepingpanel;
using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;

SafetyProperties_Peep::SafetyProperties_Peep(std::vector<ControlSystem_Peep*> cs, AllConfigArray configIn) : 
	configData(configIn), controlSystems(cs), homedCounter(0), teachingCounter(0), firstHoming(true), robotHomed(false)  {
	
	HAL& hal = HAL::instance();
	
	// Set variables depending on system configurations
	peep_direction = setPeepDirection(configData);
	enc = setEncoderInputs(configData, hal);
	readySig = setReadyInputs(configData, hal);
	
	// ############ Define critical outputs ############
	enable = hal.getLogicPeripheralOutput("enableDrv");
	criticalOutputs = { enable };
	
	// ############ Define critical inputs ############
	for (auto &e : enc)
		criticalInputs.push_back(e);
	for (auto &r : readySig)
		criticalInputs.push_back(r);
	
	// ############ Define Levels ############
	levels = {
		{ off,             "System off",                        },
		{ swInitializing,  "SW initializing",                   },
		{ swInitialized,   "SW initialized",                    },
		{ emergency,       "EMERGENCY",                         },
		{ resetEmergency,  "reset EMERGENCY",                   },
		{ controlStopping, "Stopping control system",           },
		{ controlStarting, "Starting control system",           },
		{ systemOn,        "Control System started - System On" },
		{ poweringDown,    "Powering down drives",              },
		{ poweringUp,      "Powering up drives",                },
		{ powerOn,         "Drives powered up",                 },
		{ homing,          "Homing panels",                     },
		{ homed,           "Panels homed",                      },
		{ sleeping,        "Panels sleeping (homed)"            },
		{ goingToReady,    "Going to ready position",           },
		{ ready,           "Panels are ready to peep",          },
	 	{ teaching,        "Teaching trajectory to panels",     },
	};
	
	// ############ Add events to the levels ############
	level(off             ).addEvent(doSwInit,            swInitializing,   kPublicEvent  ); 
	level(swInitializing  ).addEvent(swInitDone,          swInitialized,    kPrivateEvent );
	level(swInitialized   ).addEvent(doControlStart,      controlStarting,  kPublicEvent  );
	level(emergency       ).addEvent(doEmergencyReset,    resetEmergency,   kPublicEvent  );
	level(resetEmergency  ).addEvent(emergencyResetDone,  systemOn,         kPublicEvent  );
	level(controlStopping ).addEvent(controlStoppingDone, off,              kPrivateEvent );
	level(controlStarting ).addEvent(controlStartingDone, systemOn,         kPrivateEvent );
	level(systemOn        ).addEvent(doControlStop,       controlStopping,  kPublicEvent  ); 
	level(systemOn        ).addEvent(doPoweringUp,        poweringUp,       kPublicEvent  ); 
	level(poweringDown    ).addEvent(poweringDownDone,    controlStopping,  kPrivateEvent ); 
	level(poweringUp      ).addEvent(poweringUpDone,      powerOn,          kPrivateEvent );
	level(powerOn         ).addEvent(doPoweringDown,      poweringDownDone, kPublicEvent  ); 
	level(powerOn         ).addEvent(doHoming,            homing,           kPublicEvent  ); 
	level(homing          ).addEvent(homingDone,          homed,            kPublicEvent  ); 
	level(homed           ).addEvent(doReady,             goingToReady,     kPublicEvent  ); 
	level(sleeping        ).addEvent(doAwake,             ready,            kPublicEvent  ); 
	level(goingToReady    ).addEvent(readyDone,           ready,            kPublicEvent  ); 
	level(ready           ).addEvent(doPoweringDown,      poweringDown,     kPublicEvent  );
	level(ready           ).addEvent(doTeaching,          teaching,         kPublicEvent  );
	level(ready           ).addEvent(doReady,             goingToReady,     kPublicEvent  );
	level(ready           ).addEvent(doSleep,             sleeping,         kPublicEvent  );
	level(teaching        ).addEvent(doReady,             goingToReady,     kPublicEvent  ); 
	
	// Add events to multiple levels
	addEventToLevelAndAbove(swInitialized, doEmergency, emergency, kPublicEvent);
	
	// ############ Define input states and events for all levels ############
	level(off             ).setInputActions({ /*ignore(enc), ignore(readySig)                  */ });
	level(swInitializing  ).setInputActions({ /*ignore(enc1), ignore(ready1)                   */ });
	level(swInitialized   ).setInputActions({ /*ignore(enc1), ignore(ready1)                   */ });
	level(emergency       ).setInputActions({ /*ignore(enc1), ignore(ready1)                   */ });
	level(resetEmergency  ).setInputActions({ /*ignore(enc1), ignore(ready1)                   */ });
	level(controlStopping ).setInputActions({ /*ignore(enc1), ignore(ready1)                   */ });
	level(controlStarting ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(systemOn        ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(poweringDown    ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
// 	for (auto &e : enc)
// 		level(poweringUp      ).setInputActions({  ignore(e)/*, check(ready1, false, doEmergency)*/ });
	level(poweringUp      ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(powerOn         ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(homing          ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(homed           ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(goingToReady    ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(ready           ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(teaching        ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(sleeping        ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });

	// Define output states and events for all levels 
	level(off             ).setOutputActions({ set(enable, false) }); 
	level(swInitializing  ).setOutputActions({ set(enable, false) }); 
	level(swInitialized   ).setOutputActions({ set(enable, false) }); 
	level(emergency       ).setOutputActions({ set(enable, false) }); 
	level(resetEmergency  ).setOutputActions({ set(enable, false) }); 
	level(controlStopping ).setOutputActions({ set(enable, false) });
	level(controlStarting ).setOutputActions({ set(enable, false) });
	level(systemOn        ).setOutputActions({ set(enable, false) });
	level(poweringDown    ).setOutputActions({ set(enable, false) });
	level(poweringUp      ).setOutputActions({ set(enable, true ) });
	level(powerOn         ).setOutputActions({ set(enable, true ) });
	level(homing          ).setOutputActions({ set(enable, true ) });
	level(homed           ).setOutputActions({ set(enable, false) });
	level(sleeping        ).setOutputActions({ set(enable, false) });
	level(goingToReady    ).setOutputActions({ set(enable, true ) });  
	level(ready           ).setOutputActions({ set(enable, true ) }); 
	level(teaching        ).setOutputActions({ set(enable, false) });
	
	// *** Define and add level functions *** //

	// Boot
	level(off).setLevelAction([](SafetyContext* privateContext) {
		static bool first = true; 
		if(first == true) {
			privateContext->triggerEvent(doSwInit);
			first = false;
		}
	});
	
	level(swInitializing).setLevelAction([](SafetyContext* privateContext) {
		privateContext->triggerEvent(swInitDone); 
	});
	level(swInitialized).setLevelAction([](SafetyContext* privateContext) {
		privateContext->triggerEvent(doControlStart); 
	});
	level(controlStarting).setLevelAction([this](SafetyContext* privateContext) {
		for (auto &cs : controlSystems){
			cs->start();
		}
		privateContext->triggerEvent(controlStartingDone); 
	});
	level(controlStopping).setLevelAction([this](SafetyContext* privateContext) {
		for (auto &cs : controlSystems){
			cs->stop();
		}
		enc.clear();
		readySig.clear();
		peep_direction.clear();
		privateContext->triggerEvent(controlStoppingDone); 
	});
	level(poweringDown).setLevelAction([](SafetyContext* privateContext) {
		privateContext->triggerEvent(poweringDownDone); 
	});
	level(systemOn).setLevelAction([this](SafetyContext* privateContext) {
		static int count = 0;
		if(count > 1000){
			std::cout << controlSystems[0]->dac.getIn().getSignal().getValue() << std::endl;
			std::cout << controlSystems[1]->dac.getIn().getSignal().getValue() << std::endl;
			count = 0;
		}
		else count++;
			
		if(isDacZero()) {
			for (auto &cs : controlSystems)
				cs->dacSwitch.switchToInput(1);
			privateContext->triggerEvent(doPoweringUp);
		}
	});
	
	level(poweringUp).setLevelAction([](SafetyContext* privateContext) {
		privateContext->triggerEvent(poweringUpDone); 
	});
	level(powerOn).setLevelAction([](SafetyContext* privateContext) { 
		privateContext->triggerEvent(doHoming); 
	});
	
	level(homing).setLevelAction([this](SafetyContext* privateContext) {
		homedCounter = 0;
		static bool first = true;
		static std::array<bool,4> homed;
		
		// Set initialization speed (set speed only first time)
		if(first) {
			for(int i = 0; i < controlSystems.size(); i++){
				controlSystems[i]->pathPlanner.move(11.0);
			}
			first = false;
		}
		
		// Check if initialized
		for(int i = 0; i < controlSystems.size(); i++){
			auto torque = controlSystems[i]->inertia.getOut().getSignal().getValue();
			if(fabs(torque)>torque_init_lim) homed[i] = true;
			
			if(homed[i]){
				controlSystems[i]->pathPlanner.reset(); 
				controlSystems[i]->initAngle = controlSystems[i]->enc.getOut().getSignal().getValue();
			}
		}
		
		// Check if all axes are homed
		if(allTrue(homed)) privateContext->triggerEvent(homingDone);
	});
	
	level(homed).setLevelAction([this](SafetyContext* privateContext) {
		if(homedCounter++ > 0) { 
			if(firstHoming && !robotHomed) {
				// Set encoder offset
				for(int i = 0; i < controlSystems.size(); i++){
					controlSystems[i]->enc_offset.setValue((controlSystems[i]->initAngle - init_pos*i_gear) * peep_direction[i]);
				}
				// Set safety system variables
				firstHoming = false;
				robotHomed = true;
			}
				
			// Set init position path planner
			for (auto &cs : controlSystems)
				cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i_gear); 
			
			if(!isPosErrorZero()){
				// Set init position path planner
				for (auto &cs : controlSystems)
					cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i_gear);
			}
			else{
				// Set init position path planner
				for (auto &cs : controlSystems)
					cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i_gear);

				for (auto &cs : controlSystems){
					std::cout << "SS -> pp : " << cs->i_ref.getOut().getSignal().getValue() << std::endl;
					std::cout << "SS -> enc: " << cs->sum_enc_offset.getOut().getSignal().getValue() << std::endl;
				}
				privateContext->triggerEvent(doReady);
			}
		}
	});

	level(goingToReady).setLevelAction([this](SafetyContext* privateContext) {
		static bool first = true;
		static std::array<bool,4> ready;
		
		// Set ready position
		if(first) {
			for (auto &cs : controlSystems){
				cs->pathPlanner.setMaxSpeed(50.0);
				cs->pathPlanner.move(ready_pos, true);
			}
			first = false;
		}

		// Check if ready
		for(int i = 0; i < controlSystems.size(); i++){
			if(controlSystems[i]->pathPlanner.posReached()) ready[i] = true;
		}
		// Check if all axes are ready
		if(allTrue(ready)) privateContext->triggerEvent(readyDone);
	});
	
	// Define entry level
	entryLevel = off;
}

SafetyProperties_Peep::~SafetyProperties_Peep(){
	// nothing to do
}

bool SafetyProperties_Peep::isDacZero() {
	static bool isZero = true; static bool prev = true;
	for (auto &cs : controlSystems){
		if (cs->dacSwitch.getIn(1).getSignal().getValue() < 0.002)
			isZero = true;
		else
			isZero = false;
		prev = prev && isZero;
	}
	return prev;
}

bool SafetyProperties_Peep::isPosErrorZero() {
	bool isZero; bool prev = true;
	for (int i = 0; i < controlSystems.size(); i++){
		AxisVector refVal = controlSystems[i]->i_ref.getOut().getSignal().getValue(); 
		AxisVector actVal = controlSystems[i]->sum_enc_offset.getOut().getSignal().getValue();
		if(fabs(refVal-actVal)>err || fabs(actVal)>0.6)
			isZero = false;
		else
			isZero = true;
		prev = prev && isZero;
	}
	return prev;
}

bool SafetyProperties_Peep::allTrue(std::array<bool,4> v) {
	bool prev = true;
	for (int i = 0; i < controlSystems.size(); i++){
		bool istrue;
		if (v[i]) istrue = true;
		else      istrue = false;
		prev = prev && istrue;
	}
	return prev;
}

std::vector<double> SafetyProperties_Peep::setPeepDirection(AllConfigArray configData) {
	std::vector<double> peep_direction;
	
	if(configData[0] == 1){              // 0 0 0 1
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 2) {       // 0 0 1 0
		peep_direction.push_back(configData[8]);
	}
	else if (configData[0] == 3) {       // 0 0 1 1
		peep_direction.push_back(configData[8]);
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 4) {       // 0 1 0 0
		peep_direction.push_back(configData[5]);
	}
	else if (configData[0] == 5) {       // 0 1 0 1
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 6) {       // 0 1 1 0
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[8]);
	}
	else if (configData[0] == 7) {       // 0 1 1 1
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[8]);
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 8) {       // 1 0 0 0
		peep_direction.push_back(configData[2]);
	}
	else if (configData[0] == 9) {       // 1 0 0 1
		peep_direction.push_back(configData[11]);
		peep_direction.push_back(configData[2]);
	}		
	else if (configData[0] == 10) {      // 1 0 1 0
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[8]);
	}
	else if (configData[0] == 11) {      // 1 0 1 1
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[8]);
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 12) {      // 1 1 0 0
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[5]);
	}
	else if (configData[0] == 13) {      // 1 1 0 1
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[11]);
	}
	else if (configData[0] == 14) {      // 1 1 1 0
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[8]);
	}
	else if (configData[0] == 15) {      // 1 1 1 1 
		peep_direction.push_back(configData[2]);
		peep_direction.push_back(configData[5]);
		peep_direction.push_back(configData[8]);
		peep_direction.push_back(configData[11]);
	}
	else {
		throw eeros::EEROSException("Invalid config number");
	}
	
	return peep_direction;
}

std::vector<eeros::hal::ScalablePeripheralInput<double>*> SafetyProperties_Peep::setEncoderInputs(AllConfigArray configData, HAL& hal) {
	std::vector<eeros::hal::ScalablePeripheralInput<double>*> enc;
	
	if(configData[0] == 1){              // 0 0 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 2) {       // 0 0 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
	}
	else if (configData[0] == 3) {       // 0 0 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 4) {       // 0 1 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
	}
	else if (configData[0] == 5) {       // 0 1 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 6) {       // 0 1 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
	}
	else if (configData[0] == 7) {       // 0 1 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 8) {       // 1 0 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
	}
	else if (configData[0] == 9) {       // 1 0 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 10) {      // 1 0 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
	}
	else if (configData[0] == 11) {      // 1 0 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 12) {      // 1 1 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
	}
	else if (configData[0] == 13) {      // 1 1 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else if (configData[0] == 14) {      // 1 1 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
	}
	else if (configData[0] == 15) {      // 1 1 1 1 
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
	}
	else {
		throw eeros::EEROSException("Invalid config number");
	}
	
	return enc;
}
	
std::vector<eeros::hal::PeripheralInput<bool>*> SafetyProperties_Peep::setReadyInputs(AllConfigArray configData, HAL& hal) {
	std::vector<eeros::hal::PeripheralInput<bool>*> readySig;
	
	if(configData[0] == 1){              // 0 0 0 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 2) {       // 0 0 1 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (configData[0] == 3) {       // 0 0 1 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 4) {       // 0 1 0 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
	}
	else if (configData[0] == 5) {       // 0 1 0 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 6) {       // 0 1 1 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (configData[0] == 7) {       // 0 1 1 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 8) {       // 1 0 0 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
	}
	else if (configData[0] == 9) {       // 1 0 0 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 10) {      // 1 0 1 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (configData[0] == 11) {      // 1 0 1 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 12) {      // 1 1 0 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
	}
	else if (configData[0] == 13) {      // 1 1 0 1
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (configData[0] == 14) {      // 1 1 1 0
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (configData[0] == 15) {      // 1 1 1 1 
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else {
		throw eeros::EEROSException("Invalid config number");
	}
	
	return readySig;
}