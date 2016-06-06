#ifndef CH_NTB_DECOY_CONSTANTS_HPP_
#define CH_NTB_DECOY_CONSTANTS_HPP_

#include<cmath>
#include<eeros/math/Matrix.hpp>

namespace pathos {
	namespace decoy {
		
		// *** Math *** //
		static constexpr double pi = 3.14159265359;       // Pi
		static constexpr double g = 9.80665;              // Gravity acc    [m/s²]
		
		static constexpr double dt = 0.010;               // sample time
		static constexpr int nofAxis = 5;
		
		// *** Can *** //
		static constexpr int node_turning    = 0x01;
		static constexpr int node_swingBack  = 0x02;
		static constexpr int node_swingFront = 0x03;
		static constexpr int node_armLeft    = 0x04;
		static constexpr int node_armRight   = 0x05;
		
		static constexpr int nodes[nofAxis] = {node_turning, node_swingBack, node_swingFront, node_armLeft, node_armRight};
		static constexpr int homingMtd[nofAxis] = {35, 35, 35, 20, 21};
		
		static constexpr uint16_t statusWord    = 0x6041;
		static constexpr int oneBitMask         = 0x002F;
		static constexpr int bit12Mask          = 0x1000;
		static constexpr int bit13Mask          = 0x2000;
		static constexpr int notReadyToSwitchOn = 0x0000;
		static constexpr int readyToSwitchOn    = 0x0021;
		static constexpr int switchedOn         = 0x0023; 
		static constexpr int operationEnabled   = 0x0027; 
		static constexpr int quickStopActive    = 0x0007;   

		static constexpr int oe_homingError     = 0x2027; 
		static constexpr int oe_homingDone      = 0x1027; 
	
		// *** Motors ** //
		
		// Turn
		static constexpr int turn_homingSpeed = 50.0;
		static constexpr int turn_maxSpeed = 250.0;
		static constexpr int turn_i = 14.0;             // Gear ratio
		static constexpr int turn_i2 = 5.0;             // Driver ratio
		static constexpr int turn_encPulse = 3000.0;    // Pulses/turn
		
		// Swing
		static constexpr int swing_homingSpeed = 50;
		static constexpr int swing_maxSpeed = 2500;
		static constexpr int swing_i = 14;
		static constexpr int swing_encPulse = 3000;    // pulses/turn
		
		// Arms
		static constexpr int arm_homingSpeed = 50.0;
		static constexpr int arm_maxSpeed = 2500.0;
		static constexpr int arm_i = 23.0;
		static constexpr int arm_encPulse = 3000.0;    // pulses/turn
		
	};
};

#endif /* CH_NTB_DECOY_CONSTANTS_HPP_ */

// 		// Position
// 		static constexpr int32 positionSetPoint    = 0x6062;
// 		static constexpr int32 positionEncoderInt  = 0x6063;  // internal units
// 		static constexpr int32 positionEncoder     = 0x6064;  // user units
