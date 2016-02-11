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

MainSequence_x4::MainSequence_x4(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys, std::array<double,4> configData) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							peep_out_s(sequencer, controlSys, safetySys), peep_in_s(sequencer, controlSys, safetySys), 
							teaching_s(sequencer, controlSys, safetySys) {
	
	peep_time = configData[1];
	
	if(configData[2] == -1.0)
		peep_direction = 'r'; 
	else //configData[2] == 1.0
		peep_direction =  'l'; 

	peep_angle     = configData[3];
	
// 	peep_time      = configData[1];
// 	peep_direction = configData[2];
// 	peep_position  = configData[3];
}

bool MainSequence_x4::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_x4::run() {
	log.trace() << "Sequencer '" << name << "': started.";

	log.info() << peep_time << "; " << peep_direction << "; " << peep_angle ;
	
	while(safetySys->getCurrentLevel().getId() < ready) {    // power up & home
		usleep(100000); if (isTerminating()) return; }
	
	char m = 0;
	while (m != 'q') {
		// List options
		log.info() << "time value = set peeping time";
		log.info() << "peep value = set peeping position and peep";
		log.info() << "teach      = teach a new trajectory";
		log.info() << "quit       = exit program";
	
		std::string input_cmd, in_data;
		
		// Read input string 
		std::string line;
		std::getline(std::cin, line);
		std::size_t pos = line.find(' ');
		if (pos != std::string::npos){
			input_cmd = line.substr(0, pos); 
			in_data = line.substr(pos+1);
		} 
		else{
			input_cmd = line;
		}
		
		// Conversion string to double (if needed) and conversion l/r -> +/- 1
		double input_data;
		if(input_cmd == "time" || input_cmd == "peep") {
			std::string in = in_data;
			std::string::size_type sz;  
			input_data = std::stod(in, &sz);
		}
		else
			input_data = 0.0;
		
		// Set commands for switch case
		if(input_cmd == "time") {
			peep_time = input_data; 
		}
		else if(input_cmd == "peep") {
			peep_angle = input_data;
			
			int count = 0;
			while(count < 1){
				// Setting parameters of peeping motions
				peep_out_s.setMotionCurve("/mnt/data/curves/curve_input1.txt", peep_time, peep_angle, peep_direction);
				peep_in_s.setMotionCurve("/mnt/data/curves/curve_input1.txt" , peep_time);
				// Peep 
				peep_out_s();
				peep_in_s(); 
				
				sleep(1.0);
				count++;
			}	
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
}

void MainSequence_x4::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_x4::isTerminating() {
	return sequencer->getState() == state::terminating;
}