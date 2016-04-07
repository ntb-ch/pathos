#include "canopen-faulhaber-drv.h"
#include "canopen-com.h"

int init_can_nodes(int sock){
	
	// start nodes
	send_nmt(sock, CANOPEN_NMT_MC_CS_START, 0);
  
}

// initializes a motor
int init_faulhaber_motor(int sock, int node)
{
	int err = 0;
	
	if((err = canopen_sdo_download_exp(sock, node, 0x6040, 0, 0x0008, 2)) != 0){	// Shutdown
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, 0x6040, 0, 0x0006, 2)) != 0){	// Shutdown too?
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, 0x6040, 0, 0x0007, 2)) != 0){	// Switch on
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, 0x6040, 0, 0x000F, 2)) != 0){	// Enable Operation
		return err;
	}

	uint32_t data;
	printf("Status: ");
	if((err = canopen_sdo_upload_exp(sock, node, 0x6041,0, &data)) != 0){
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
	if((err = canopen_sdo_download_exp(sock, node, 0x6060, 0, 0x0006, 0)) != 0){
		return err;
	}

	// Homing at current position
	if((err = canopen_sdo_download_exp(sock, node, 0x6098, 0, 35, 2)) != 0){
		return err;
	}	
	// Referenzfahrt starten
	if((err = canopen_sdo_download_exp(sock, node, 0x4060, 0, 0x001F, 0)) != 0){	
		return err;
	}
	
	if((err = canopen_sdo_download_exp(sock, node, 0x6060, 0, 0x0001, 1)) != 0){	//change to profile position mode
		return err;
	}
	return 0;
}

int set_ramp_faulhaber(int sock, int node, int speed)
{
	int err = 0;
	
	// Rampenspeed in rmp/s²
	if((err = canopen_sdo_download_exp(sock, node, 0x6083, 0, speed, 0)) != 0){
		return err;
	}
	if((err = canopen_sdo_download_exp(sock, node, 0x6084, 0, speed, 0)) != 0){
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
	return canopen_sdo_download_exp(sock, node, 0x607F, 0, speed, 0);

}