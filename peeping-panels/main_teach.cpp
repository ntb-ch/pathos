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

#include "control/ControlSystem_Teach.hpp"
#include "safety/SafetyProperties_Teach.hpp"
#include "sequences/MainSequence_teach.hpp"
#include "config/CreateControlSystems.hpp"
#include "config/LoadSetConfig.hpp"

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
	std::array<double,13> allPanelsData;
	std::vector<Array3> configSystem;
	eeros::math::Matrix<16,15,int> allSensAreas;
// 	LoadConfig_system("/mnt/data/curves/configsystem.txt", &allPanelsData, &configSystem, &allSensAreas);
		
	// Create Control Systems
	std::vector<ControlSystem_Teach*> controlSystems = CreateControlSystem_teach(allPanelsData);   

	// Create the safety system
	SafetyProperties_Teach safetyProperties(controlSystems, allPanelsData);
	SafetySystem safetySystem(safetyProperties, dt);
	
	// Initialize logger
	Logger<LogWriter>::setDefaultWriter(&w);
	
	// Get and Start Sequencer
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_teach*> mainSequences;
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_teach(sequencers[i], controlSystems[i], &safetySystem));
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
			if (sequencers[i]->getState() == state::terminated){ 
				istrue = true; 
				std::cout << "terminated" << std::endl;
				
			}
			else      istrue = false;
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
	while(safetySystem.getCurrentLevel().getId() > off) 
		usleep(100000);
	safetySystem.shutdown();

	// Remove elements from vectors
	controlSystems.clear();
	sequencers.clear();
	mainSequences.clear();
	
	return 0;
}