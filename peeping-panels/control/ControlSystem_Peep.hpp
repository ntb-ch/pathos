#ifndef CH_NTB_PEEPINGPANEL_CONTROLSYSTEMPEEP_HPP_
#define CH_NTB_PEEPINGPANEL_CONTROLSYSTEMPEEP_HPP_
 
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/D.hpp>
#include <eeros/control/Sum.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/DeMux.hpp>
#include <eeros/control/Switch.hpp>
#include <eeros/control/Saturation.hpp>

#include "PathPlanner.hpp"
#include "ReadSensorData.hpp"
#include "../sensors/SensorsThread.hpp"

namespace pathos {
	namespace peepingpanel {
 
		class ControlSystem_Peep {
		
			public:
				ControlSystem_Peep(std::string enc, std::string dac, SensorsThread* sensThread, std::array<int,60> sensAreas);

				void start(); // starts timedomains             
				void stop();  // stops timedomains
			
				// Define blocks here
				AxisVector initAngle;
				
				pathos::peepingpanel::ReadSensorData sensorsInputData;
				
				eeros::control::PeripheralInput<double> enc;
				eeros::control::Mux<1,double, AxisVector> mux_enc;
				
				eeros::control::Constant<AxisVector> enc_offset;
				eeros::control::Sum<2,AxisVector> sum_enc_offset;
				eeros::control::D<AxisVector> encSpeed;
				
				pathos::peepingpanel::PathPlanner pathPlanner;
				eeros::control::Gain<AxisVector,AxisVector> i_ref;
				eeros::control::D<AxisVector> refSpeed;
				
				eeros::control::Sum<2,AxisVector> sumPos;
				eeros::control::Gain<AxisVector,AxisVector> kp;
				
				eeros::control::Sum<2,AxisVector> sumVelRef;
				eeros::control::Sum<2,AxisVector> sumVel;
				eeros::control::Gain<AxisVector,AxisVector> kv;
				
				eeros::control::Gain<AxisVector,AxisVector> inertia;
				eeros::control::Gain<AxisVector,AxisVector> inv_km;
				
				eeros::control::Constant<AxisVector> speedInit; 
				eeros::control::Switch<2,AxisVector> speedSwitch;
				
				eeros::control::DeMux<1,double,AxisVector> demux_dac;
				eeros::control::Saturation<double> dac_saturation;
				
				eeros::control::Constant<double> dacConst;
				eeros::control::Switch<2,double> dacSwitch;
				eeros::control::PeripheralOutput<double> dac;
				
			//private:
				eeros::control::TimeDomain timedomain;
				
				AxisVector pos, posPrev;
				
		};  // end class  
	};
}; 
 
#endif //CH_NTB_PEEPINGPANEL_CONTROLSYSTEMPEEP_HPP_