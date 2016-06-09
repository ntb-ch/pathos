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

void homing_motor(int socket, int node, int homingMethod){
	int err = 0;
	
// 	err = send_nmt(socket, CANOPEN_NMT_MC_CS_START, node);
	if(err != 0){
		throw eeros::EEROSException("send_nmt homing failed");
	}
	if((err = homing_faulhaber_motor(socket, node, homingMethod)) != 0){
		throw eeros::EEROSException("homing failed");
	}
}

void set_position_profile_mode(int socket){
	int err = 0;
	
	for(int i=0;i<nofAxis;i++){
// 		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
		
		if((err = set_position_profile_mode_faulhaber(socket, nodes[i])) != 0){
			throw eeros::EEROSException("set position profile mode failed");
		}
	}
}

void set_motion_parameters(int socket){
	int err = 0;
	
	for(int i=0;i<nofAxis;i++){
// 		send_nmt(socket, CANOPEN_NMT_MC_CS_START, nodes[i]);
		
		if((err = set_acc_dec_faulhaber(socket, nodes[i], 10000,10000)) != 0){  

			throw eeros::EEROSException("set ramp failed");
		}
		if((err = set_max_speed_faulhaber(socket, nodes[i], 3000)) != 0){
			throw eeros::EEROSException("set max nodes failed");            
		}
	}
}

#endif // CH_NTB_DECOY_MOTORFUNCTIONS_HPP_  

