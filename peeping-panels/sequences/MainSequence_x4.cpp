#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_x4.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "../control/PeepingPanelControlSystem.hpp"
#include <eeros/core/EEROSException.hpp>
#include "../constants.hpp"
#include "../PeepingPanelConfig.hpp"

#include <unistd.h>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace pathos::peepingpanel;
using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;
// using namespace eeros::math;

MainSequence_x4::MainSequence_x4(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							peep_out_s(sequencer, controlSys, safetySys), peep_in_s(sequencer, controlSys, safetySys), 
							teaching_s(sequencer, controlSys, safetySys) {
	// nothing to do
}

bool MainSequence_x4::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_x4::run() {
	log.trace() << "Sequencer '" << name << "': started.";
	
	// Loading configuration file
	log.info() << "Loading configuration file"; 
	std::string fileName("/mnt/data/curves/config.txt");        
	PeepingPanelConfig configFile(fileName.c_str());
	configFile.load();
	
	// Set default variables
	peep_speed = configFile.peep_speed;
	if(configFile.peep_direction == 1.0 || configFile.peep_direction == -1.0)
		peep_direction = configFile.peep_direction;
	else  {
		peep_direction = 1.0;
		log.warn() << "Wrong direction input"; 
	}
	peep_position = configFile.peep_position;
	
	log.info() << "Parameters loaded: "; 
	log.info() << "Peep speed: " << peep_speed << "\t Peep direction: " << peep_direction << "\t Peep position: "  << peep_position ;
		
	while(safetySys->getCurrentLevel().getId() < ready) {    // power up & home
		usleep(100000); if (isTerminating()) return; }
	
	char m = 0;
	while (m != 'q') {
		// List options
		log.info() << "speed     value = set peeping speed";
		log.info() << "direction r/l   = set peeping direction";
		log.info() << "peep      value = set peeping position and peep";
		log.info() << "teach           = teach a new trajectory";
		log.info() << "quit            = exit program";
	
		std::string input_cmd, in_data;
		
		// Read input string 
		std::string line;
		std::getline(std::cin, line);
		std::size_t pos = line.find(' ');
		if (pos != std::string::npos) {
			input_cmd = line.substr(0, pos); 
			in_data = line.substr(pos+1);
		} 
		else {
			input_cmd = line;
		}
		
		// Conversion string to double (if needed) and conversion l/r -> +/- 1
		double input_data;
		if(input_cmd == "speed" || input_cmd == "peep" || input_cmd == "peepshy" || input_cmd == "peepfast" || input_cmd == "peepslow") {
			std::string in = in_data;
			std::string::size_type sz;  
			input_data = std::stod(in, &sz);
		}
		else if(input_cmd == "direction") {
			if(in_data == "l")
				input_data =  1.0;
			else
				input_data = -1.0;
		}
		else
			input_data = 0.0;
		
		// Set commands for switch case
		if(input_cmd == "speed") {
			peep_speed = input_data; 
			controlSys->pathPlanner.setMaxSpeed(peep_speed);
		}
		else if(input_cmd == "direction") {
			peep_direction = input_data;
// 			peep_out_s.setPeepDirection(peep_direction);
		}
		else if(input_cmd == "peep") {
			peep_position = input_data;
			
			// set parameters
			controlSys->pathPlanner.setMaxSpeed(peep_speed);
			
			int count = 0;
			while(count < 3){
				// peep 
				peep_out_s.setMotionCurve("/mnt/data/curves/curve_input1.txt", 1.0, 1.04, 'r');
				peep_in_s.setMotionCurve("/mnt/data/curves/curve_input1.txt", 1.0);
				
				peep_out_s();
				peep_in_s(); 
				
				sleep(1.0);
				count++;
			}
			m = 'q';
			log.info() << "finished sequence";
		}
		else if(input_cmd == "teach") {
			safetySys->triggerEvent(doTeaching);  
			while(safetySys->getCurrentLevel().getId() < teaching) {    
				usleep(100000); if (isTerminating()) return; }
			teaching_s();    
		}
		else if(input_cmd == "quit") {
			m = 'q';
		}
		else
			log.info() << "Invalid command, try again";
	}

	// Save parameters to config file
	log.info() << "Do you want to save parameters on config file? [yes/no]";
		
	// Read input string
	std::string line;
	std::getline(std::cin, line);	
	if(line == "yes") {
		// save parameters
		configFile.peep_direction = peep_direction;
		configFile.peep_position = peep_position;
		configFile.peep_speed = peep_speed;
		configFile.save();
		
		log.info() << "Parameters saved: ";
		log.info() << "Config peep speed: "     << configFile.peep_speed     ;
		log.info() << "Config peep direction: " << configFile.peep_direction ;
		log.info() << "Config peep position: "  << configFile.peep_position  ;
	}
	else
		log.info() << "Parameters not saved";
}

void MainSequence_x4::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_x4::isTerminating() {
	return sequencer->getState() == state::terminating;
}