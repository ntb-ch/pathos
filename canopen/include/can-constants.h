#ifndef _CAN_CONSTANTS_H_
#define _CAN_CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	uint16_t statusWord  = 0x6041;
	
	uint16_t controlWord     = 0x6040;
	uint32_t shutDown        = 0x0006;
	uint32_t switchOn        = 0x0007;
	uint32_t enableOperation = 0x000F;

	uint16_t modeOfOperation = 0x6060;
	
	uint16_t profileAcceleration = 0x6083;
	uint16_t profileDeceleration = 0x6084;
	uint16_t maxAllowedSpeed = 0x607F;
	
	uint8_t homingMode = 0x6098;

#ifdef __cplusplus
}
#endif

#endif /* _CAN_CONSTANTS_H_ */