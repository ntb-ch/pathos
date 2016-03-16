#include <unistd.h>
#include <signal.h>
#include <iostream>

#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp> 
#include <eeros/logger/SysLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/core/EEROSException.hpp>
#include <pathos/Hardware.hpp>
#include <eeros/core/Executor.hpp>

#include "control/ControlSystem_Teach.hpp"
#include "safety/SafetyProperties_Teach.hpp"
#include "sequences/MainSequence_teach.hpp"
#include "config/CreateControlSystems.hpp"
#include "config/LoadSetConfig.hpp"
#include "types.hpp"

using namespace eeros::logger;
using namespace eeros::control;
using namespace eeros::safety;
using namespace eeros::sequencer;
using namespace pathos;
using namespace pathos::peepingpanel;

volatile bool running = true;
void signalHandler(int signum) {
	running = false;
}

int main() {
	signal(SIGINT, signalHandler);
	StreamLogWriter w(std::cout);
	Logger<LogWriter> log('M');
	
	// Initializing hardware
	Hardware h;
	
	// Load configuration files
	AllConfigArray allPanelsData;
	LoadConfig_Teach("/mnt/data/curves/configsystem.txt", &allPanelsData);
		
	// Start sensors data thread
	SensorsThread sensorsThread;
	while(!sensorsThread.isRunning()) usleep(100000);
	
	// Create Control Systems
	std::vector<ControlSystem_Teach*> controlSystems = CreateControlSystem_teach(allPanelsData);   
	
	// Initialize logger
	Logger<LogWriter>::setDefaultWriter(&w);
	
	auto &executor = Executor::instance();
	executor.setPeriod(dt);
	
	// Create the safety system
	SafetyProperties_Teach safetyProperties(controlSystems, allPanelsData);
	SafetySystem safetySystem(safetyProperties, dt);
		
	executor.setMainTask(safetySystem);
	
	// Get and Start Sequencer
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_teach*> mainSequences;
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_teach(sequencers[i], controlSystems[i], &safetySystem));
		sequencers[i]->start(mainSequences[i]);
	}
	
	executor.run();
	
	// Safe control system shut down
	for (int i = 0; i < controlSystems.size(); i++){
		controlSystems[i]->dacSwitch.switchToInput(0);
		controlSystems[i]->timedomain.run();
	}
	// Shut down safety system
	safetySystem.triggerEvent(doPoweringDown);
	while(safetySystem.getCurrentLevel().getId() > off) {
	  usleep(2000);
	  safetySystem.run();
	}
	// Remove elements from vectors
	controlSystems.clear();
	sequencers.clear();
	mainSequences.clear();
	
	// Stop sensors communication
	sensorsThread.stop();
	sensorsThread.join();
	
	return 0;
}