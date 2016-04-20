#include <iostream>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/core/Executor.hpp>
#include "control/CanHandle.hpp"
#include "control/ControlSystem_decoy.hpp"
#include "safety/SafetyProperties_decoy.hpp"
#include "sequences/MainSequence_decoy.hpp"
#include <signal.h>
#include <canopen-com.h>

using namespace eeros;
using namespace eeros::logger;
using namespace eeros::safety;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace pathos::decoy;
 
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
	ControlSystem_decoy* canCtrlSys;
	double dt = 0.001;
	
	canHandle = new CanHandle("can0");
	canCtrlSys = new ControlSystem_decoy(canHandle->getSocket(), dt);
		
	// Create safety system
	SafetyProperties_decoy safetyProperties(canCtrlSys);
	SafetySystem safetySystem(safetyProperties, dt);
	
	// Set executor & create safety system
	auto &executor = Executor::instance();
	executor.setPeriod(dt);
	executor.setMainTask(safetySystem);
	
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
	
	// Sequencer
	Sequencer sequencer;
	MainSequence_decoy mainSequence(&sequencer, canCtrlSys, &safetySystem);
	sequencer.start(&mainSequence);
	
	// Start control system
	executor.run();
	
	log.info() << "Shuting down...";
	
	return 0;

}
