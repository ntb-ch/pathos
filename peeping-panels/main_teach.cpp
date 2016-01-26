#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <vector>

#include <eeros/hal/HAL.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp> 
#include <eeros/logger/SysLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <pathos/Hardware.hpp>
#include <eeros/core/EEROSException.hpp>

#include "control/PeepingPanelControlSystem.hpp"
#include "safety/PeepingPanelSafetyProperties_x4.hpp"
#include "sequences/MainSequence_teach.hpp"
#include "PeepingPanelConfig.hpp"
#include "constants.hpp"

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
	
	// Create vector of control systems and sequencers
	std::vector<PeepingPanelControlSystem*> controlSystems;
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_teach*> mainSequences;
	
	// Initializing hardware
	Hardware h;
	
	// Create Control System
	controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
	int config = 8;

	Logger<LogWriter>::setDefaultWriter(&w);

	// Create the safety system
	PeepingPanelSafetyProperties_x4 safetyProperties(config, controlSystems);
	SafetySystem safetySystem(safetyProperties, dt);
	
	// Get and Start Sequencer
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_teach(sequencers[i], controlSystems[i], &safetySystem));
		sequencers[i]->start(mainSequences[i]);
	}
	
	bool isTerminated = false;
	bool runningSeq = true;
	while(running && runningSeq) {
		usleep(1000000);
		// Chek if all sequence have terminated
		bool prev = true;
		for (int i = 0; i < sequencers.size(); i++){
			bool istrue;
			if (sequencers[i]->getState() == state::terminated) istrue = true;
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