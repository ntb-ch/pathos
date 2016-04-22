#include "../include/canopen-faulhaber-drv.h"
#include "../include/canopen-com.h"
#include "../include/can-constants.h"

int init_can_nodes(int sock){
	
	// start nodes
	send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0);
  
}

// initializes a motor
int init_faulhaber_motor(int sock, int node)
{
	int err = 0;
	
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, shutDown, 2)) != 0){        // Shut down
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, switchOn, 2)) != 0){        // Switch on
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, controlWord, 0, enableOperation, 2)) != 0){ // Enable Operation
		return err;
	}

	uint32_t data;
	printf("Status: ");
	if((err = canopen_sdo_upload_exp(sock, node, statusWord,0, &data)) != 0){
		printf("bad state\n");
		return err;
	}
	printf("0x%.4x\n", data);
	if(data != 0x37){
		return -5; 			// drive not enabled
	}
	return 0;
}

int homing_faulhaber_motor(int sock, int node)
{
	int err = 0;
	// In den Homing-Mode wechseln
	if((err = canopen_sdo_download_exp(sock, node, modeOfOperation, 0, 0x0006, 0)) != 0){
		return err;
	}

	// Homing at current position
	if((err = canopen_sdo_download_exp(sock, node, homingMode, 0, 35, 2)) != 0){       
		return err;
	}	
	// Referenzfahrt starten
	if((err = canopen_sdo_download_exp(sock, node, 0x4060, 0, 0x001F, 0)) != 0){	
		return err;
	}
	
	if((err = canopen_sdo_download_exp(sock, node, modeOfOperation, 0, 0x0001, 1)) != 0){	//change to profile position mode
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