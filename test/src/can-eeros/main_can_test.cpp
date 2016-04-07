#include <iostream>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>

#include "control/CanHandle.hpp"
#include "control/CanControlSystem.hpp"

#include <signal.h>

#include <canopen-com.h>

using namespace eeros::logger;
using namespace eeros::control;
 
volatile bool running = true;

void signalHandler(int signum){
	running = false;
}

int main() {
	signal(SIGINT, signalHandler);
      
	StreamLogWriter w(std::cout);
	Logger<LogWriter>::setDefaultWriter(&w);
	Logger<LogWriter> log;
 
	CanHandle* canHandle;
	CanControlSystem* canCtrlSys;
	double dt = 0.001;
	
 	canHandle = new CanHandle("can0");
	canCtrlSys = new CanControlSystem(canHandle->getSocket(), dt);
	
	
	//init of Faulhaber Motor/Drive
	int err = 0;
	if((err = init_faulhaber_motor(canHandle->getSocket(), 0x05)) < 0){
		throw eeros::EEROSException("init of motor 0x05 failed");
	}
	if((err = homing_faulhaber_motor(canHandle->getSocket(), 0x05)) < 0){
		throw eeros::EEROSException("homing of motor 0x05 failed");
	}
	if((err = set_ramp_faulhaber(canHandle->getSocket(), 0x05, 10000)) < 0){
		throw eeros::EEROSException("set ramp of motor 0x05 failed");
	}
	if((err = set_max_speed_faulhaber(canHandle->getSocket(), 0x05, 3000)) < 0){
		throw eeros::EEROSException("set max speed of motor 0x05 failed");
	}
	
	uint32_t dataStatus;
	std::cout << "Status: " << std::endl;
	if((err = canopen_sdo_upload_exp(canHandle->getSocket(), 0x05, 0x6041,0, &dataStatus)) != 0){
		std::cout << "Error init of drive" << std::endl;
	}
	std::cout << "status after init: " << std::endl;
	if((dataStatus & 0xFF) != 0x37){
		std::cout << "not enabled" << std::endl;// drive not enabled
	}
	std::cout << "success" << std::endl;
	
	usleep(10000);
	
	std::cout << "init motor 0x05 successfull" << std::endl;
	
	
	// start control system
	canCtrlSys->start();
	
	
	canCtrlSys->setPos.setValue(0);
	usleep(10000);
	canCtrlSys->setPos.setValue(1000);
	usleep(10000);
	canCtrlSys->setPos.setValue(-1000);
	
	int cnt = 0;
	bool neg = false;
	int16_t drvCtrl = 0;
	int32_t data = 0;
	bool ready = false;
	
	log.info() << "starting...";
	
	while(running){
		cnt++;
		if(ready && (cnt % 400 == 0)){
			cnt = 0;
			if(neg){
				canCtrlSys->setPos.setValue(16000);
				neg = false;
			}
			else{
				canCtrlSys->setPos.setValue(-16000);
				neg = true;
			}
			canCtrlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO2_TX);
		}
		if(ready){
			canCtrlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO2_TX);
			if(canCtrlSys->canReceive.getPdoValue(0x05, CANOPEN_FC_PDO2_TX, &drvCtrl, &data)!=0){
				log.info() << "function code not found";
			}
			else{
				std::cout << std::hex << "ctrl: " << drvCtrl << std::dec << "\t data: " << data << std::endl; 
			}
		}
		else {
			canCtrlSys->canSend.initiatePdoRequest(0x05, CANOPEN_FC_PDO1_TX);
			if(canCtrlSys->canReceive.getPdoValue(0x05, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){
				log.info() << "function code not found";
			}
			else{
				if( (drvCtrl & 0x002F) == 0x0027 ){
					ready = true;
					std::cout << std::hex << "status: " << drvCtrl << std::endl; 
				}
				else{
					std::cout << std::hex << "status: " << drvCtrl << std::endl; 
				}
			}
		}
		usleep(10000);
	}
	
	log.info() << "Shuting down...";
	
	canCtrlSys->stop();
		
	return 0;
}
