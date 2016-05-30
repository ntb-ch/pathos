#ifndef CH_NTB_CANSAFETYPROPERTIES_HPP_
#define CH_NTB_CANSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/ScalablePeripheralInput.hpp>
#include <eeros/hal/FlinkWatchdog.hpp>
	
	class CanControlSystem;
	
	// Define events
	enum {
	  doOff = 1,
	};
	
	// Define levels
	enum {
	  off = 1,
	};
	
	class CanSafetyProperties : public eeros::safety::SafetyProperties {

		public:
			CanSafetyProperties(CanControlSystem* cs);
			virtual ~CanSafetyProperties();
		
			// outputs
			// inputs
		
		private:
			CanControlSystem* controlSys;
	};

	#endif // CH_NTB_CANSAFETYPROPERTIES_HPP_