#include "RadToPulses.hpp"
#include "../constants.hpp"

using namespace eeros::control;
using namespace pathos::decoy;

RadToPulses::RadToPulses(double enc_pulses, double i_gear) : encPulses(enc_pulses), gear(i_gear) { }

RadToPulses::~RadToPulses() { 
	// nothing to do...
}

void RadToPulses::run() {
	
	int output = (int)(in.getSignal().getValue() * encPulses * gear / (2.0 * pi) );
	
	out.getSignal().setValue(output);
	out.getSignal().setTimestamp(in.getSignal().getTimestamp());
}

Input<double>& RadToPulses::getIn(){
	return in;
}

Output<int>& RadToPulses::getOut() {
	return out;
}