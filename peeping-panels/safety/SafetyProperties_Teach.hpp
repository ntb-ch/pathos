#ifndef CH_NTB_PEEPINGPANEL_SAFETYPROPERTIESTEACH_HPP_
#define CH_NTB_PEEPINGPANEL_SAFETYPROPERTIESTEACH_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/ScalablePeripheralInput.hpp>
#include <eeros/hal/PeripheralInput.hpp>
#include "../types.hpp"

namespace pathos {
	namespace peepingpanel{
	
		class ControlSystem_Teach;
		
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
			doSleep = 22,
			doAwake = 23,
			doReady = 24,
			readyDone = 25,
			doTeaching = 26,
			teachingDone = 27,
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
			sleeping = 21,
			homed = 22, 
			goingToReady = 23,
			ready = 24,
			teaching = 25,
		};
		
		class SafetyProperties_Teach : public eeros::safety::SafetyProperties {

			public:
				SafetyProperties_Teach(std::vector<ControlSystem_Teach*> cs, std::array<double,13> configIn);
				virtual ~SafetyProperties_Teach();
			
				// critical outputs
				eeros::hal::PeripheralOutput<bool>* enable;
				
				// critical inputs
				std::vector<eeros::hal::ScalablePeripheralInput<double>*> enc;
				std::vector<eeros::hal::PeripheralInput<bool>*> readySig;

			private:
				std::vector<ControlSystem_Teach*> controlSystems;
				                         
				virtual bool isDacZero();
				virtual bool allTrue(std::array<bool,4> v);
				virtual bool isPosErrorZero();
				virtual std::vector<double> setPeepDirection(std::array<double,13> configData);
				virtual std::vector<eeros::hal::ScalablePeripheralInput<double>*> setEncoderInputs(std::array<double,13> configData, eeros::hal::HAL& hal);
				virtual std::vector<eeros::hal::PeripheralInput<bool>*> setReadyInputs(std::array<double,13> configData, eeros::hal::HAL& hal);
				
				std::array<double,13> configData;
				std::vector<double> peep_direction;
				double err = 0.00001;
		};
	};
};
#endif // CH_NTB_PEEPINGPANEL_SAFETYPROPERTIESTEACH_HPP_