#include <eeros/safety/SafetySystem.hpp>
#include "MainSequence_peep.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include "../control/PeepingPanelControlSystem.hpp"
#include <unistd.h>

using namespace pathos::peepingpanel;
using namespace eeros;
using namespace eeros::sequencer;
using namespace eeros::safety;

MainSequence_peep::MainSequence_peep(Sequencer* sequencer, PeepingPanelControlSystem* controlSys, SafetySystem* safetySys) :
							Sequence<void>("main", sequencer), controlSys(controlSys), safetySys(safetySys),
							peep_out_s(sequencer, controlSys, safetySys), peep_in_s(sequencer, controlSys, safetySys) {
	// nothing to do
}

bool MainSequence_peep::checkPreCondition() {
	return safetySys->getCurrentLevel().getId() >= off;
}

void MainSequence_peep::run() {
	log.trace() << "[ Main Sequence Started ]";

	while(safetySys->getCurrentLevel().getId() < ready) {    // power up & home
		usleep(100000); if (isTerminating()) return; }

	// PARAMETERS TO SET
	double peep_time = 1.0;
	double peep_angle = 1.04;
	char peep_direction = 'r';
		
	// Set parameters (file name, time, angle, peep direction)
	peep_out_s.setMotionCurve("/mnt/data/curves/curve_input1.txt", peep_time, peep_angle, peep_direction);
	peep_in_s.setMotionCurve("/mnt/data/curves/curve_input1.txt" , peep_time);

	// Run peeping sequences
	peep_out_s();
	peep_in_s(); 
	
	sleep(1.0);
}

void MainSequence_peep::exit() {
	log.info() << "[ Exit Main Sequence ]";
}

inline bool MainSequence_peep::isTerminating() {
	return sequencer->getState() == state::terminating;
}