#ifndef CH_NTB_PATHOS_PEEPINGPANEL_CONSTANTS_HPP_
#define CH_NTB_PATHOS_PEEPINGPANEL_CONSTANTS_HPP_

#include<cmath>
#include<eeros/math/Matrix.hpp>

#include "types.hpp"

namespace pathos {
	namespace peepingpanel {
		
		static constexpr double pi = 3.14159;
		static constexpr double dt = 0.001; 
		static constexpr unsigned int nofAxis = 1;
		
		static constexpr double i_gear = 6.4;                                            // Gear ratio
		static constexpr double km = 25.1*pow(10.0, -3.0);                               // Motor constant [Nm/A]
		static constexpr double J_motor = 135.0*pow(10.0, -7.0) + 1.70*pow(10.0, -4.0);  // Motor+Additional Mass inertia [kgm²]
		
		static eeros::math::Matrix<1,1,double> velMax =  2.0;                            // Max velocity [rad/s]
		static eeros::math::Matrix<1,1,double> accMax =  2.0;                            // Max acceleration [rad/s²]
		static eeros::math::Matrix<1,1,double> decMax = -2.0;                            // Max acceleration [rad/s²]
		
		static eeros::math::Matrix<1,1,double> pos_ctrl_gain = 80.0;                     // Position control gain
		static eeros::math::Matrix<1,1,double> vel_ctrl_gain = 20.0; // 120.0;           // Position control gain
		
		static constexpr double pos_init_lim    = -11.0;
		static constexpr double torque_init_lim =   0.2;
		static eeros::math::Matrix<1,1,double> ready_pos = 0.0;
		static eeros::math::Matrix<1,1,double> init_pos = -1.0 * (5.0) * (pi/180.0); // (4.0)
		
	}
}

#endif /* CH_NTB_PATHOS_PEEPINGPANEL_CONSTANTS_HPP_ */