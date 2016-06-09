#ifndef CH_NTB_DECOY_SAFETYPROPERTIES_HPP_
#define CH_NTB_DECOY_SAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/ScalablePeripheralInput.hpp>
#include <eeros/hal/FlinkWatchdog.hpp>

#include <canopen-faulhaber-drv.h>
#include "../constants.hpp"

namespace pathos {
	namespace decoy{
		
		class ControlSystem_decoy;
		
		// Define events
		enum {
			doOff = 1,
			doInitMotors = 2,
			initMotorsDone = 3,
			doDisable = 10,
			doEnable = 11,
			enablingDone = 12,
			doHome = 14,
			homingDone1 = 15,
			homingDone2 = 16,
			homingDone3 = 17,
			homingDone4 = 18,
			homingDone5 = 19,
			setMotionParam = 20,
			doReady = 30,
		};
		
		// Define levels
		enum {
			off = 1,
			initMotors = 2,
			motorsEnabling = 3,
			motorsEnabled = 4,
			homing1 = 15,
			homing2 = 16,
			homing3 = 17,
			homing4 = 18,
			homing5 = 19,
			homed = 20,
			settingMotionParam = 21,
			ready = 30,
		};
		
		class SafetyProperties_decoy : public eeros::safety::SafetyProperties {

			public:
				SafetyProperties_decoy(int canSock, ControlSystem_decoy* cs);
				virtual ~SafetyProperties_decoy();
			
			private:
				int socket;
				ControlSystem_decoy* controlSys;
	
		};
	};
};

#endif // CH_NTB_DECOY_SAFETYPROPERTIES_HPP_