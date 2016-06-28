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
 
	w.show();
	
	// Can handle
	CanHandle* canHandle;
	canHandle = new CanHandle("can0");
	
	// Start logger
	Logger<LogWriter>::setDefaultWriter(&w);	
	
	// Control system
	ControlSystem_decoy* controlSystem;
	controlSystem = new ControlSystem_decoy(canHandle->getSocket(), dt);
	
	// Create safety system
	SafetyProperties_decoy safetyProperties(canHandle->getSocket(), controlSystem);
	SafetySystem safetySystem(safetyProperties, dt);
	
	// Set executor & create safety system
	auto &executor = Executor::instance();
	executor.setPeriod(dt);
	executor.setMainTask(safetySystem);
	
	// Sequencer
	Sequencer sequencer;
	MainSequence_decoy mainSequence(&sequencer, controlSystem, &safetySystem);
	sequencer.start(&mainSequence);
	
	// Start control system
	executor.run();
	
	sequencer.shutdown();
	sleep(3);
	if(sequencer.getState()!=state::terminated) 
		sequencer.abort();
	
	canHandle->~CanHandle();
	
	return 0;

}
