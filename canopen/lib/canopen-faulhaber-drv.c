#include "../include/canopen-faulhaber-drv.h"
#include "../include/canopen-com.h"
#include "../include/can-constants.h"
#include <stdio.h>

int init_can_nodes(int sock){
	
	// start nodes
	return send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0);
  
}

int init_faulhaber_motor(int sock, int node)
{
	int err = 0;
      
	//TODO
// 	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, 0x0008,        2)) != 0) {        
// 		return err-20;                                                                        
// 	}
	/*if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, 0x0000,        2)) != 0) {        
		return err-20;                                                                        
	}*/	
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, shutDown,        2)) != 0) {        
		return err-20;                                                                        
	}                                                                                         
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, switchOn,        2)) != 0) {        
		return err-30;                                                                        
	}                                                                                       
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, enableOperation, 2)) != 0) { 
		return err-40;
	}

	return 0;
}

int setspeed_homing_faulhaber_motor(int sock, int node, int speed)
{
	int err = 0;
	if(speed > 0 && speed <= 1000){
		if((err = canopen_sdo_download_exp(sock, node, setHomingSpeed, 1, speed, 0)) != 0){
			return err;
		}
		if((err = canopen_sdo_download_exp(sock, node, setHomingSpeed, 2, speed, 0)) != 0){       
			return err;
		}
	}
	else {
		err = 100;
		return err;
	}
}

int homing_faulhaber_motor(int sock, int node, int homingMethod)
{
	int err = 0;
	
	// Change to homing mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, homingMode, 1)) != 0){
		return err;
	}
	
	// Set homing method
	if((err = canopen_sdo_download_exp(sock, node, setHomingMethod, 0, homingMethod, 1)) != 0){       
		return err;
	}
	
	// Start reference path
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, enOp_startHoming, 2)) != 0){ 
		return err;
	}
	
// // 	// Change to profile position mode
// // 	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, positionMode, 1)) != 0){
// // 		return err;
// // 	}
	
	return 0;
}

int set_profile_velocity_mode_faulhaber(int sock, int node){
	int err = 0;
	
	//Velocity-mode	
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, speedMode, 1)) != 0){
		return err;
	}
}

int set_position_profile_mode_faulhaber(int sock, int node)
{
	int err = 0;
	uint32_t data;
	
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, positionMode, 1)) != 0){
		return err;
	}
	
	if((err = canopen_sdo_upload_exp(sock, node, 0x6061, 0, &data)) != 0){
		return err;
	}
// 	printf("mode: %d\n", data);
	
	return 0;
}

int set_interpolated_position_mode_drive(int sock, int node)
{
	int err = 0;
	
	//interpolated position mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, interpolatedPositionMode, 1)) != 0){
		return err;
	}
}

int set_acc_dec_faulhaber(int sock, int node, int acc, int dec) 
{
	int err = 0;
	
	// Rampenspeed in rmp/s²
	if(acc > 0 && acc < 30000 && dec > 0 && dec < 30000){
		if((err = canopen_sdo_download_exp(sock, node, profileAcceleration, 0, acc, 0)) != 0){
			return err;
		}
		if((err = canopen_sdo_download_exp(sock, node, profileDeceleration, 0, dec, 0)) != 0){
			return err;
		}
	}
	else {
		err = 100;
		return err;
	}
	return 0;
}

int set_target_speed_faulhaber(int sock, int node, int32_t speed) // speed in rpm
{
	int err = 0;
	
	// Change to speed mode
	if((err = canopen_sdo_download_exp(sock, node, switchModeOfOperation, 0, speedMode, 1)) != 0){
		return err;
	}
	// Set target velocity
	if((err = canopen_sdo_download_exp(sock, node, targetSpeed, 0, speed, 0)) != 0){
		return err;
	}
	
	return 0;
}

int set_max_speed_faulhaber(int sock, int node, int speed)    // speed in rpm
{
	int err = 0;
	
	// Set max velocity
	if((err = canopen_sdo_download_exp(sock, node, maxAllowedSpeed, 0, speed, 0)) != 0){
		return err;
	}

	return 0;
}

int get_warning_drive(int sock, int node, uint32_t *warningData)
{
	int err = 0;
	
	if((err = canopen_sdo_upload_exp(sock, node, warning, 0, warningData)) != 0){
		return err;
	}
		
	return 0;
}

int get_status_drive(int sock, int node, uint32_t *status)
{
	int err = 0;
	
	if((err = canopen_sdo_upload_exp(sock, node, statusWord, 0, status)) != 0){
		return err;
	}
		
	return 0;
}

int configure_sync_drive(int sock, int node, uint32_t period, uint16_t limit){
	int err = 0;
	
	if((err = canopen_sdo_download_exp(sock, node, comCyclePeriod, 0, period, 4)) != 0){
		return err;
	}
	
	err = 0;
	if((err = canopen_sdo_download_exp(sock, node, syncControl, 4, limit, 2)) != 0){
		return err;
	}
	
	return 0;
}
