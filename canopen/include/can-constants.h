#ifndef _CAN_CONSTANTS_H_
#define _CAN_CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	uint16_t statusWord  = 0x6041;
	
	uint16_t controlWord      = 0x6040;
	uint32_t shutDown         = 0x0006;
	uint32_t switchOn         = 0x0007;
	uint32_t faultReset       = 0x0008;
	uint32_t enableOperation  = 0x000F;
	uint32_t enOp_startHoming = 0x001F; 

	uint16_t switchModeOfOperation = 0x6060; 
	uint32_t homingMode            = 0x0006;
	uint32_t positionMode          = 0x0001;
	uint32_t velocityMode          = 0x0003;
	
	uint16_t profileAcceleration = 0x6083;
	uint16_t profileDeceleration = 0x6084;
	uint16_t maxAllowedSpeed     = 0x607F;
	
	uint16_t homingMethod = 0x6098;
	uint16_t homingSpeed  = 0x6099;
	uint16_t homingAcc    = 0x609A;

#ifdef __cplusplus
}
#endif

#endif /* _CAN_CONSTANTS_H_ */

// homingMode
//	  1 to 14: homing with index pulse
//	 17 to 30: homing without index pulse
//	 33 to 34: homing at index pulse
//	 35: homing at actual position