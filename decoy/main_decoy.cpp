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
#include <canopen-faulhaber-drv.h>
#include <fcntl.h>

#include "constants.hpp"

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
	Logger<LogWriter> log('M');
 
	// Can handle
	CanHandle* canHandle;
	canHandle = new CanHandle("can0");
	
	// Start logger
	Logger<LogWriter>::setDefaultWriter(&w);
		
// 	is_switch_active(canHandle->getSocket(), node_armLeft, );
	
	std::cout << "before control sys" << std::endl;
	
	// Control system
	ControlSystem_decoy* controlSystem;
	controlSystem = new ControlSystem_decoy(canHandle->getSocket(), dt);
	
	std::cout << "after control sys" << std::endl;
	
	// Create safety system
	SafetyProperties_decoy safetyProperties(canHandle->getSocket(), controlSystem);
	SafetySystem safetySystem(safetyProperties, dt);
	
	std::cout << "after safety sys" << std::endl;

	// Set executor & create safety system
	auto &executor = Executor::instance();
	executor.setPeriod(dt);
	executor.setMainTask(safetySystem);
	
	std::cout << "after settings exe" << std::endl;

	// Sequencer
	Sequencer sequencer;
	MainSequence_decoy mainSequence(&sequencer, controlSystem, &safetySystem);
	sequencer.start(&mainSequence);
	
	std::cout << "after sequencer" << std::endl;

	// Start control system
	executor.run();
	std::cout << "after exe run" << std::endl;

	sequencer.shutdown();
	sleep(3);
	if(sequencer.getState()!=state::terminated) 
		sequencer.abort();
	
	
	
	canHandle->~CanHandle();
// 	log.info() << "Shuting down..."; // TODO not working
	
	return 0;

}
