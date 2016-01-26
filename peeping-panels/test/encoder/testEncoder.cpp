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

#include <eeros/control/PeripheralInput.hpp>

// #include "constants.hpp"

using namespace eeros::logger;
using namespace eeros::hal;
using namespace pathos;
// using namespace pathos::peepingpanel;

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
// 	Logger<LogWriter>::setDefaultWriter(&w);
	
	// Initializing hardware
	Hardware h;
		
	HAL& hal = HAL::instance();
	auto &enc1 = *hal.getRealPeripheralInput("enc1");
	
	double gear = 6.4;
	
// 	auto &enableDrv = *hal.getLogicPeripheralOutput("enableDrv");

	while(running) {
		std::cout << enc1.get() << "; " << enc1.get() / gear << std::endl;
		usleep(1000000);
	}
} 

