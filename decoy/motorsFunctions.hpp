#ifndef CH_NTB_DECOY_MOTORFUNCTIONS_HPP_
#define CH_NTB_DECOY_MOTORFUNCTIONS_HPP_

#include <iostream>
#include <canopen-com.h>
#include <canopen-faulhaber-drv.h>
#include "constants.hpp"

using namespace pathos::decoy;

void init_motors(int socket){
	int err = 0;
	
	for(int i=0;i<nofAxis;i++){
		if((err = send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i])) != 0){
			throw eeros::EEROSException("send_nmt on init failed.");
		}
		
		if((err = init_faulhaber_motor(socket, nodes[i])) != 0){
			throw eeros::EEROSException("init failed");
		}
	}
}

void homing_motors(int socket){
	int err = 0;
	
// 	for(int i=0;i<nofAxis;i++){
// 		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
// 		
// 		if((err = homing_faulhaber_motor_endSwitch(socket, nodes[i], homingMtd[i])) != 0){
// 			throw eeros::EEROSException("homing failed");
// 		}
// 	}
	err = send_nmt(socket, CANOPEN_NMT_MC_CS_START, node_armLeft);
	if(err != 0){
		throw eeros::EEROSException("send_nmt homing failed");
	}
	if((err = homing_faulhaber_motor(socket, node_armLeft, 20)) != 0){
		throw eeros::EEROSException("homing failed");
	}
}

void set_position_profile_mode(int socket){
	int err = 0;
	
	for(int i=0;i<nofAxis;i++){
		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
		
		if((err = set_position_profile_mode_faulhaber(socket, nodes[i])) != 0){
			throw eeros::EEROSException("set position profile mode failed");
		}
	}
}

void set_motion_parameters(int socket){
	int err = 0;
	
	for(int i=0;i<nofAxis;i++){
		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
		
		if((err = set_acc_dec_faulhaber(socket, nodes[i], 10000,10000)) != 0){  
			throw eeros::EEROSException("set ramp failed");
		}
		if((err = set_max_speed_faulhaber(socket, nodes[i], 3000)) != 0){
			throw eeros::EEROSException("set max nodes failed");            
		}
	}
}

// OLD
// void initializeMotors(int socket){
// 	int err = 0;
// 	int nodes[nofAxis] = {node_turning, node_swingFront, node_swingBack, node_armLeft, node_armRight};
// 	
// 	for(int i=0;i<nofAxis;i++){
// 		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
// 		
// 		if((err = init_faulhaber_motor(socket, nodes[i])) != 0){
// 			throw eeros::EEROSException("init failed");
// 		}
// 		if((err = homing_faulhaber_motor_actualPos(socket, nodes[i])) != 0){
// 			throw eeros::EEROSException("homing failed");
// 		}
// 		if((err = set_ramp_faulhaber(socket, nodes[i], 10000)) != 0){       // 10000 = (int) speed
// 			throw eeros::EEROSException("set ramp failed");
// 		}
// 		if((err = set_max_speed_faulhaber(socket, nodes[i], 3000)) != 0){
// 			throw eeros::EEROSException("set max nodes failed");       // 3000  = (int) speed
// 		}
// 		
// 		uint32_t dataStatus;
// 	// 	std::cout << "Status: " << std::endl;
// 	// 	if((err = canopen_sdo_upload_exp(socket, nodes[i], statusWord,0, &dataStatus)) != 0){
// 	// 		std::cout << "Error init of drive" << std::endl;
// 	// 	}
// 		std::cout << "status after init: ";
// 		if((dataStatus & 0xFF) != 0x37){
// 			std::cout << "not enabled" << std::endl;// drive not enabled
// 		}
// 	// 	std::cout << "success" << std::endl;
// 		
// 		usleep(10000);
// 		std::cout << "init node " << nodes[i] << " successfull" << std::endl;
// 	}
// }


#endif // CH_NTB_DECOY_MOTORFUNCTIONS_HPP_  

