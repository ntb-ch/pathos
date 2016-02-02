#include "../control/PeepingPanelControlSystem.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "Teaching.hpp"
#include <eeros/safety/SafetySystem.hpp>
#include <unistd.h>
#include "../constants.hpp"

#include <iostream>
#include <fstream>

using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros::sequencer;
using namespace eeros::safety;

Teaching::Teaching(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys) : 
		Sequence<>("main", sequencer), controlSys(controlSys), safetySys(safetySys) {
	// nothing to do
}

void Teaching::init() {
	std::bind(&Teaching::init, *this);
}

bool Teaching::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() == teaching;
}

void Teaching::run() {
	log.info() << "[ Teaching ] started";
	
	// Get file name
	log.info() << "Enter file name, where you want to save your trajectory points (path/filename.txt)";
	// Read input string 
	std::string line;
	std::getline(std::cin, line);
	
	log.info() << "Enter how long you want to save data (seconds)";
	// Read input string 
	std::string seconds_str;
	std::getline(std::cin, seconds_str);
	// Convert to double
	std::string::size_type sz;  
	int seconds = std::stod(seconds_str, &sz);
	
	std::cout << seconds << std::endl;
	
	std::ofstream teachingFile;
	teachingFile.open(line);  
	
	char m = 0;
	int count = 0;
	double time = 0;
	while (m != 'q') {
		teachingFile << time << "\t";
		teachingFile << controlSys->enc.getOut().getSignal().getValue() << "\n";
		usleep(dt*1000000); // plot data each dt
		
		time = time + dt;
		count++;
		if(count > (seconds/dt)){
			m = 'q'; 
			count = 0;
		}
		if (isTerminating()) return;
	}
	teachingFile.close();

	safetySys->triggerEvent(doReady);
	while(safetySys->getCurrentLevel().getId() < ready) {    
		usleep(100000); if (isTerminating()) return; }
}

bool Teaching::checkPostCondition() {
	return safetySys->getCurrentLevel().getId() == ready;
}

void Teaching::exit() {
	log.info() << "[ Teaching ] exit done";
}

inline bool Teaching::isTerminating() {
	return sequencer->getState() == state::terminating;
}

// inline bool Teaching::isStopping() {
// 	return controlSys->keyboard.esc.getSignal().getValue();
// }