#include "../include/canopen-faulhaber-drv.h"
#include "../include/canopen-com.h"
#include "../include/can-constants.h"

int init_can_nodes(int sock){
	
	// start nodes
	send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0);
	
// 	send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0x05);
// 	
// 	send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0x04);
  
}

// initializes a motor
int init_faulhaber_motor(int sock, int node)
{
	int err = 0;

	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, 0x000E, 2)) != 0){          // Decoy BA ???
		return err-10;
	}
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, shutDown, 2)) != 0){        
		return err-20;
	}
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, switchOn, 2)) != 0){        
		return err-30;
	}
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, enableOperation, 2)) != 0){ 
		return err-40;
	}

	return 0;
}

int homing_faulhaber_motor_actualPos(int sock, int node)
{
	int err = 0;
	
	// Change to homing mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, homingMode, 0)) != 0){
		return err;
	}
	// Homing at current position
	if((err = canopen_sdo_download_exp(sock, node, homingMethod, 0, 35, 2)) != 0){       
		return err;
	}
	// Change to profile position mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, positionMode, 1)) != 0){
		return err;
	}
	
	return 0;
}

int homing_faulhaber_motor_endSwitch(int sock, int node)
{
	int err = 0;
	
	// Change to homing mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, homingMode, 0)) != 0){
		return err;
	}
	// Homing at current position
	if((err = canopen_sdo_download_exp(sock, node, homingMethod, 0, 35, 2)) != 0){       
		return err;
	}
	// Homing at lower endswitch
	if((err = canopen_sdo_download_exp(sock, node, homingMethod, 0, 20, 2)) != 0){       
		return err;
	}	
	// Start reference path
	if((err = canopen_sdo_download_exp(sock, node, statusWord, 0, enOp_startHoming, 0)) != 0){
		return err;
	}
	// Change to profile position mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, positionMode, 1)) != 0){
		return err;
	}
	
	return 0;
}

int set_ramp_faulhaber(int sock, int node, int speed)
{
	int err = 0;
	
	// Rampenspeed in rmp/s²
	if((err = canopen_sdo_download_exp(sock, node, profileAcceleration, 0, speed, 0)) != 0){
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, profileDeceleration, 0, speed, 0)) != 0){
		return err;
	}
	
	return 0;
}

// in rpm
int set_max_speed_faulhaber(int sock, int node, int speed)
{
	// In den Homing-Mode wechseln
	//sdo_download(0x6081, 0, speed, 1);

	// Neue position setzen
	return canopen_sdo_download_exp(sock, node, maxAllowedSpeed, 0, speed, 0);

}