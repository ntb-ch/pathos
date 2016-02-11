#ifndef CH_NTB_PEEPINGPANEL_SAFETYPROPERTIES_HPP_
#define CH_NTB_PEEPINGPANEL_SAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/ScalablePeripheralInput.hpp>
#include <eeros/hal/PeripheralInput.hpp>
#include "../types.hpp"

namespace pathos {
	namespace peepingpanel{
	
		class PeepingPanelControlSystem;
		
		// Define events
		enum {
		doOff = 1,
		doSwInit = 2,
		swInitDone = 3,
		doEmergency = 4,
		doEmergencyReset = 5,
		emergencyResetDone = 6, 
		doControlStart = 7,
		doControlStop = 8,
		controlStartingDone = 9, 
		controlStoppingDone = 10, 
		doPoweringUp = 11,
		poweringUpDone = 12,
		doPoweringDown = 13, 
		poweringDownDone = 14,
		doHoming = 20,
		homingDone = 21,
		doReady = 22,
		readyDone = 23,
		doTeaching = 24,
		teachingDone = 25,
		doSwShutDown = 40,
		};
		
		// Define levels
		enum {
		off = 1,
		swInitializing = 2,
		swInitialized = 3,
		emergency = 4,
		resetEmergency = 5,
		controlStopping = 6, 
		controlStarting = 7, 
		systemOn = 8,
		poweringDown = 9, 
		poweringUp = 10,
		powerOn = 11,
		homing = 20, 
		homed = 21, 
		goingToReady = 22,
		ready = 23,
		teaching  =24,
		};
		
		class PeepingPanelSafetyProperties_x4 : public eeros::safety::SafetyProperties {

			public:
				PeepingPanelSafetyProperties_x4(std::vector<PeepingPanelControlSystem*> cs, std::array<double,4> configData);
				virtual ~PeepingPanelSafetyProperties_x4();
			
				// critical outputs
				eeros::hal::PeripheralOutput<bool>* enable;
				
				// critical inputs
				std::vector<eeros::hal::ScalablePeripheralInput<double>*> enc;
				std::vector<eeros::hal::PeripheralInput<bool>*> readySig;

			private:
				std::vector<PeepingPanelControlSystem*> controlSystems;
				                         
				virtual bool isDacZero();
				virtual bool allTrue(std::array<bool,4> v);
				virtual bool isPosErrorZero();
				
				std::array<double,4> configData;
				double err = 0.00001;
				char buffer[1024];	
		};
	};
};
#endif // CH_NTB_PEEPINGPANEL_SAFETYPROPERTIES_HPP_