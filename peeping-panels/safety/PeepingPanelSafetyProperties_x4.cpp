#include "PeepingPanelSafetyProperties_x4.hpp"
#include "../control/PeepingPanelControlSystem.hpp"

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

PeepingPanelSafetyProperties_x4::PeepingPanelSafetyProperties_x4(int config, std::vector<PeepingPanelControlSystem*> cs) : 
	controlSystems(cs), config(config)  {
		
	HAL& hal = HAL::instance();

	// ############ Define critical outputs ############
	enable = hal.getLogicPeripheralOutput("enableDrv");
	
	criticalOutputs = { enable };
	
	// ############ Define critical inputs ############

	if(config == 1){               // 0 0 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 2) {        // 0 0 1 0
		config = 2;
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (config == 3) {       // 0 0 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 4) {       // 0 1 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
	}
	else if (config == 5) {       // 0 1 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 6) {       // 0 1 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (config == 7) {      // 0 1 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 8) {     // 1 0 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
	}
	else if (config == 9) {     // 1 0 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 10) {     // 1 0 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (config == 11) {      // 1 0 1 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 12) {     // 1 1 0 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
	}
	else if (config == 13) {     // 1 1 0 1
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else if (config == 14) {    // 1 1 1 0
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
	}
	else if (config == 15) {    // 1 1 1 1 
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3")));
		enc.push_back(dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4")));
		readySig.push_back(hal.getLogicPeripheralInput("readySig1"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig2"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig3"));
		readySig.push_back(hal.getLogicPeripheralInput("readySig4"));
	}
	else {
		throw eeros::EEROSException("Invalid config number");
	}
	
	for (auto &e : enc)
			criticalInputs.push_back(e);
	for (auto &r : readySig)
			criticalInputs.push_back(r);

// 	enc1  = dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc1"));
// 	enc2  = dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc2"));
// 	enc3  = dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc3"));
// 	enc4  = dynamic_cast<ScalablePeripheralInput<double>*>(hal.getRealPeripheralInput("enc4"));
// 	
// 	ready1 = hal.getLogicPeripheralInput("readySig1");
// 	ready2 = hal.getLogicPeripheralInput("readySig2");
// 	ready3 = hal.getLogicPeripheralInput("readySig3");
// 	ready4 = hal.getLogicPeripheralInput("readySig4");
	
// 	criticalInputs = { enc1, enc2, enc3, enc4, ready1, ready2, ready3, ready4};
	
	// ############ Define Levels ############
	levels = {
		{ off,             "System off",                       },
		{ swInitializing,  "SW initializing",                  },
		{ swInitialized,   "SW initialized",                   },
		{ emergency,       "EMERGENCY",                        },
		{ resetEmergency,  "reset EMERGENCY",                  },
		{ controlStopping, "Stopping control system",          },
		{ controlStarting, "Starting control system",          },
		{ systemOn,        "Control System started",           },
		{ poweringDown,    "Powering down drives",             },
		{ poweringUp,      "Powering up drives",               },
		{ powerOn,         "Drives powered up",                },
		{ homing,          "Homing panels",                    },
		{ homed,           "Panels homed",                     },
		{ goingToReady,    "Going to ready position",          },
		{ ready,           "Panels are ready to peep",         },
	 	{ teaching,        "Teaching trajectory to panels",    },
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
	level(goingToReady    ).addEvent(readyDone,           ready,            kPublicEvent  ); 
	level(ready           ).addEvent(doPoweringDown,      poweringDown,     kPublicEvent  );
	level(ready           ).addEvent(doTeaching,          teaching,         kPublicEvent  );
	level(ready           ).addEvent(doReady,             goingToReady,     kPublicEvent  );
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
	level(poweringUp      ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(powerOn         ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(homing          ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(homed           ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(goingToReady    ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(ready           ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });
	level(teaching        ).setInputActions({ /*ignore(enc1), check(ready1, false, doEmergency)*/ });

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
	level(goingToReady    ).setOutputActions({ set(enable, true ) });  
	level(ready           ).setOutputActions({ set(enable, true ) }); 
	level(teaching        ).setOutputActions({ set(enable, false) });
	
	// *** Define and add level functions *** //

	int &homedCounter     = * new int; homedCounter     = 0;
	int &teachingCounter  = * new int; teachingCounter  = 0;
	int &balancingCounter = * new int; balancingCounter = 0;
	int &highSpeedCounter = * new int; highSpeedCounter = 0;
	int &stopMoveCounter  = * new int; stopMoveCounter  = 0;
	
	static bool firstHoming = true;
	static bool robotHomed  = false;
	
	// Boot
	level(off).setLevelAction([&](SafetyContext* privateContext) {
		static bool first = true; 
		if(first == true) {
			privateContext->triggerEvent(doSwInit);
			first = false;
		}
// 		// Initialize serial communication
// 		p.block(true);
// 		p.baud(serial::Baud::b115200);
	});
	
	level(swInitializing).setLevelAction([&](SafetyContext* privateContext) {
		privateContext->triggerEvent(swInitDone); 
	});
	level(swInitialized).setLevelAction([&](SafetyContext* privateContext) {
		privateContext->triggerEvent(doControlStart); 
	});
	level(controlStarting).setLevelAction([&](SafetyContext* privateContext) {
		for (auto &cs : controlSystems){
			cs->start();
		}
		privateContext->triggerEvent(controlStartingDone); 
	});
	level(controlStopping).setLevelAction([&](SafetyContext* privateContext) {
		for (auto &cs : controlSystems){
			cs->stop();
		}
		enc.clear();
		readySig.clear();
		privateContext->triggerEvent(controlStoppingDone); 
	});
	level(poweringDown).setLevelAction([&](SafetyContext* privateContext) {
		privateContext->triggerEvent(poweringDownDone); 
	});
	level(systemOn).setLevelAction([&](SafetyContext* privateContext) {
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
	
	level(poweringUp).setLevelAction([&](SafetyContext* privateContext) {
		privateContext->triggerEvent(poweringUpDone); 
	});
	level(powerOn).setLevelAction([&](SafetyContext* privateContext) { // TODO see with sequencer
		privateContext->triggerEvent(doHoming); 
	});
	
	level(homing).setLevelAction([&](SafetyContext* privateContext) {
		homedCounter = 0;
		static bool first = true;
		static std::array<bool,4> homed;
		
		// Set initialization speed (set speed only first time)
		if(first) {
			for (auto &cs : controlSystems)
				cs->speedInit.setValue(initialization_speed);
			first = false;
		}
			
		// Check if initialized
		for(int i = 0; i < controlSystems.size(); i++){
			auto dac = controlSystems[i]->dac.getIn().getSignal().getValue();
			if(dac<-0.8 || dac>0.8) homed[i] = true;
			
			if(homed[i]){
				controlSystems[i]->speedInit.setValue(0.0);
				controlSystems[i]->initAngle = controlSystems[i]->enc.getOut().getSignal().getValue();
			}
		}
		
// 		static int count = 0;
// 		if(count > 1000){
// 			std::cout << homed[0] << "; " << homed[1] << "; " << homed[2] << "; " << homed[3] << std::endl;
// 			count = 0;
// 		}
// 		else count++;
		
		// Check if all axes are homed
		if(allTrue(homed)) privateContext->triggerEvent(homingDone);
	});
	
	level(homed).setLevelAction([&](SafetyContext* privateContext) {
		if(homedCounter++ > 0) { 
			if(firstHoming && !robotHomed) {
				// Set encoder offset
				for (auto &cs : controlSystems)
					cs->enc_offset.setValue( -cs->initAngle + init_pos * i);
				
				// Set safety system variables
				firstHoming = false;
				robotHomed = true;
			}
			
			// Set init position path planner
			for (auto &cs : controlSystems)
				cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i); 
			
			if(!isPosErrorZero()){
				// Set init position path planner
				for (auto &cs : controlSystems)
					cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i);
			}
			else{
				// Set init position path planner
				for (auto &cs : controlSystems)
					cs->pathPlanner.setInitPos(cs->sum_enc_offset.getOut().getSignal().getValue() / i);
				
				// Switch to position control
				for (auto &cs : controlSystems)
					cs->speedSwitch.switchToInput(1);

				for (auto &cs : controlSystems){
					std::cout << "SS -> pp : " << cs->i_ref.getOut().getSignal().getValue() << std::endl;
					std::cout << "SS -> enc: " << cs->sum_enc_offset.getOut().getSignal().getValue() << std::endl;
				}
				privateContext->triggerEvent(doReady);
			}
		}
	});

	level(goingToReady).setLevelAction([&](SafetyContext* privateContext) {
		static bool first = true;
		static std::array<bool,4> ready;
		
		// Set ready position
		if(first) {
			for (auto &cs : controlSystems){
				cs->pathPlanner.setMaxSpeed(30.0);
				cs->pathPlanner.move(ready_pos, true);
			}
			first = false;
		}

		// Check if ready
		for(int i = 0; i < controlSystems.size(); i++){
			if(controlSystems[i]->pathPlanner.posReached()) ready[i] = true;
		}
		
// 		static int count = 0;
// 		if(count > 1000){
// 			std::cout << ready[0] << "; " << ready[1] << "; " << ready[2] << "; " << ready[3] << std::endl;
// 			count = 0;
// 		}
// 		else count++;
		
		// Check if all axes are ready
		if(allTrue(ready)) privateContext->triggerEvent(readyDone);
	});
	
	// Define entry level
	entryLevel = off;
}

PeepingPanelSafetyProperties_x4::~PeepingPanelSafetyProperties_x4(){
	// nothing to do
}

bool PeepingPanelSafetyProperties_x4::isDacZero() {
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

bool PeepingPanelSafetyProperties_x4::isPosErrorZero() {
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

bool PeepingPanelSafetyProperties_x4::allTrue(std::array<bool,4> v) {
	bool prev = true;
	for (int i = 0; i < controlSystems.size(); i++){
		bool istrue;
		if (v[i]) istrue = true;
		else      istrue = false;
		prev = prev && istrue;
	}
	return prev;
}

