#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <eeros/hal/HAL.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp> 
#include <eeros/logger/SysLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <pathos/Hardware.hpp>
#include <eeros/core/EEROSException.hpp>

#include "../../control/PeepingPanelControlSystem.hpp"
#include "../../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "MainSequence_behaviour.hpp"
#include "../../constants.hpp"
#include "../../PeepingPanelConfig.hpp"

using namespace eeros::logger;
using namespace eeros::hal;
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
	
	// Create vector of control systems and sequencers
	std::vector<PeepingPanelControlSystem*> controlSystems;
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_behaviour*> mainSequences;
	int config = 8;
	
	// Initializing hardware
	Hardware h;
	
	// *** Control System *** //
	controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
	
	Logger<LogWriter>::setDefaultWriter(&w);
	
	// *** Loading configuration file *** //
	std::cout << "Loading configuration file"  << std::endl; 
	std::string fileName("/mnt/data/curves/config.txt");        
	PeepingPanelConfig configFile(fileName.c_str());
	configFile.load();
		
	std::array<double, 4> configArray; // motorconfig, time, direction, position
	// config motors
	configArray[0] = config;
	// peep time
	configArray[1] = configFile.peep_time;
	// peep direction
	if(configFile.peep_direction == 1.0 || configFile.peep_direction == -1.0) {
		configArray[2] = configFile.peep_direction;
	}
	else
		throw EEROSException("Wrong direction input, check input file");
	// peep position
	configArray[3] = configFile.peep_position;
	
	// *** Create the safety system *** //
	PeepingPanelSafetyProperties_x4 safetyProperties(controlSystems, configArray);
	SafetySystem safetySystem(safetyProperties, dt);
	
	
	// *** Get and Start Sequencer *** //
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_behaviour(sequencers[i], controlSystems[i], &safetySystem, configArray));
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
// 	sequencers.clear();
	mainSequences.clear();
	
	return 0;
} 