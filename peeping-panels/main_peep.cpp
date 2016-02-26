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

#include "control/ControlSystem_Peep.hpp"
#include "safety/SafetyProperties_Peep.hpp"
#include "sequences/MainSequence_peep.hpp"
#include "config/CreateControlSystems.hpp"
#include "config/LoadSetConfig.hpp"
#include "sensors/SensorsThread.hpp"

using namespace eeros::logger;
using namespace eeros::hal;
using namespace pathos;
using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros::safety;
using namespace eeros::sequencer;

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
	std::array<double,13> allPanelsData;
	std::array<int,60> sensArea_1, sensArea_2, sensArea_3, sensArea_4;
	std::vector<Array3> configSystem;
	LoadConfig_system("/mnt/data/curves/configsystem.txt", &allPanelsData, &configSystem, &sensArea_1, &sensArea_2, &sensArea_3, &sensArea_4);
	
	// Start sensors data thread
	SensorsThread sensorsThread;
	while(!sensorsThread.isRunning()) usleep(100000);
	
	// Create Control Systems
	std::vector<ControlSystem_Peep*> controlSystems = CreateControlSystem(allPanelsData, sensArea_1, sensArea_2, sensArea_3, sensArea_4, &sensorsThread);   
	
	// Initialize logger
	Logger<LogWriter>::setDefaultWriter(&w);
	
	// Check: Nr. control systems and settings for sequencer has to match
	if(configSystem.size() != controlSystems.size())
		throw EEROSException("Wrong settings, program will be stopped");
	
	// Create the safety system
	SafetyProperties_Peep safetyProperties(controlSystems, allPanelsData);
	SafetySystem safetySystem(safetyProperties, dt);
	
	// Get Sequencer
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_peep*> mainSequences;
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_peep(sequencers[i], controlSystems[i], &safetySystem, configSystem.at(i)));
		sequencers[i]->start(mainSequences[i]);
	}
	
	// Stay in the loop while the program is running
	bool isTerminated = false;
	bool runningSeq = true;
	while(running && runningSeq) {
		usleep(1000000);
		// Chek if all sequence have terminated
		bool prev = true;
		for (int i = 0; i < sequencers.size(); i++){
			bool istrue;
			if (sequencers[i]->getState() == state::terminated) istrue = true;
			else  istrue = false;
			prev = prev && istrue;
		}
		runningSeq = !prev;
	}

	// Safe control system shut down
	for (int i = 0; i < controlSystems.size(); i++){
		controlSystems[i]->dacSwitch.switchToInput(0);
		controlSystems[i]->stop();
	}
	// Shut down safety system
	safetySystem.triggerEvent(doPoweringDown);
	while(safetySystem.getCurrentLevel().getId() > off) usleep(100000);
	safetySystem.shutdown();
	
	// Remove elements from vectors
	configSystem.clear();
	controlSystems.clear();
	sequencers.clear();
	mainSequences.clear();
	
	// Stop sensors communication
	sensorsThread.stop();
	sensorsThread.join();
	
	return 0;
}