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
#include "sequences/MainSequence_x4.hpp"
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

int main(int argc, char *argv[]) {
	signal(SIGINT, signalHandler);
	StreamLogWriter w(std::cout);
	Logger<LogWriter> log('M');
	
	// Check if input argument is missing
	if(argc < 2)
		throw eeros::EEROSException("Missing input argument - config mode");
	
	// Create vector of control systems and sequencers
	std::vector<PeepingPanelControlSystem*> controlSystems;
	std::vector<Sequencer*> sequencers;
	std::vector<MainSequence_x4*> mainSequences;
	int config = 0;
	
	// Initializing hardware
	Hardware h;
	
	// Read input argument & Create Control Systems
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == 'a') {
			config = 0;
			throw eeros::EEROSException("No motor connected");
		}
		else if (argv[i][0] == 'b') {        // 0 0 0 1
			std::cout << "Motor 4 connected" << std::endl;
			config = 1; 
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'c') {        // 0 0 1 0
			std::cout << "Motor 3 connected" << std::endl;
			config = 2;
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
		}
		else if (argv[i][0] == 'd') {       // 0 0 1 1
			std::cout << "Motor 3 and 4 connected" << std::endl;
			config = 3; 
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'e') {       // 0 1 0 0
			std::cout << "Motor 2 connected" << std::endl;
			config = 4;
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
		}
		else if (argv[i][0] == 'f') {       // 0 1 0 1
			std::cout << "Motor 2 and 4 connected" << std::endl;
			config = 5; 
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'g') {       // 0 1 1 0
			std::cout << "Motor 2 and 3 connected" << std::endl;
			config = 6;
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
		}
		else if (argv[i][0] == 'h') {      // 0 1 1 1
			std::cout << "Motor 2, 3 and 4 connected" << std::endl;
			config = 7;
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'i') {     // 1 0 0 0
			std::cout << "Motor 1 connected" << std::endl;
			config = 8;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
		}
		else if (argv[i][0] == 'j') {     // 1 0 0 1
			std::cout << "Motor 1 and 4 connected" << std::endl;
			config = 9;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'k') {     // 1 0 1 0
			std::cout << "Motor 1 and 3 connected" << std::endl;
			config = 10;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
		}
		else if (argv[i][0] == 'l') {      // 1 0 1 1
			std::cout << "Motor 1, 3 and 4 connected" << std::endl;
			config = 11;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'm') {     // 1 1 0 0
			std::cout << "Motor 1 and 2 connected" << std::endl;
			config = 12;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
		}
		else if (argv[i][0] == 'n') {     // 1 1 0 1
			std::cout << "Motor 1, 2 and 4 connected" << std::endl;
			config = 13;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else if (argv[i][0] == 'o') {    // 1 1 1 0
			std::cout << "Motor 1, 2 and 3 connected" << std::endl;
			config = 14;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
		}
		else if (argv[i][0] == 'p') {    // 1 1 1 1 
			std::cout << "All motors connected" << std::endl;
			config = 15;
			controlSystems.push_back(new PeepingPanelControlSystem("enc1", "dac1"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc2", "dac2"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc3", "dac3"));
			controlSystems.push_back(new PeepingPanelControlSystem("enc4", "dac4"));
		}
		else {
			config = 0;
			throw eeros::EEROSException("Invalid input argument: 'a' to 'p'");
		}
	}

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
	
	Logger<LogWriter>::setDefaultWriter(&w);
	
	// *** Get and Start Sequencer *** //
	for (int i = 0; i < controlSystems.size(); i++){
		sequencers.push_back(new Sequencer);
		mainSequences.push_back(new MainSequence_x4(sequencers[i], controlSystems[i], &safetySystem, configArray));
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