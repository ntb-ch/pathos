#ifndef CH_NTB_DECOY_SAFETYPROPERTIES_HPP_
#define CH_NTB_DECOY_SAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/ScalablePeripheralInput.hpp>
#include <eeros/hal/FlinkWatchdog.hpp>

namespace pathos {
	namespace decoy{
		
		class ControlSystem_decoy;
		
		// Define events
		enum {
		doOff = 1,
		};
		
		// Define levels
		enum {
		off = 1,
		};
		
		class SafetyProperties_decoy : public eeros::safety::SafetyProperties {

			public:
				SafetyProperties_decoy(ControlSystem_decoy* cs);
				virtual ~SafetyProperties_decoy();
			
				// outputs
				// inputs
			
			private:
				ControlSystem_decoy* controlSys;
		};
	};
};

#endif // CH_NTB_DECOY_SAFETYPROPERTIES_HPP_