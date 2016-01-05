#include <iostream>
#include <ostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include <eeros/hal/HAL.hpp>
#include <eeros/hal/FlinkDigOut.hpp>
#include <eeros/hal/FlinkDigIn.hpp>
#include <eeros/hal/FlinkAnalogOut.hpp>
#include <eeros/hal/FlinkFqd.hpp>
#include <eeros/hal/FlinkPwm.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>

#include <pathos/Hardware.hpp>

#define FPGA_DEVICE "/dev/flink0"

/*unique id: 0x1790x00y
		|  | --> y: number of block
		|  |
		|   --> x: block type
		|
		|
		--> 0x179: GHUEP: sum of hex ascii values as unique start
*/
	  
#define FQD_ID 			2 	//unique id: 0x17901001
#define DAC_ID 			1  	//unique id: 0x17902001
#define GPIO_ENABLE_READY_ID 	4	//0x17903001
#define IO_PWM_ID 		3	//0x17904001

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::logger;
using namespace pathos;


int main() {
	std::cout << "PATHOS PeepingPanels Control Board Testing " << std::endl;
	StreamLogWriter w(std::cout);
	Logger<LogWriter>::setDefaultWriter(&w);
	
	Hardware hw;
	HAL& hal = HAL::instance();
	
	std::cout << "Setup Done!" << std::endl;
	
	auto &dac1 = *hal.getRealPeripheralOutput("dac1");
	auto &dac2 = *hal.getRealPeripheralOutput("dac2");
	auto &dac3 = *hal.getRealPeripheralOutput("dac3");
	auto &dac4 = *hal.getRealPeripheralOutput("dac4");
	double dac1val = 0.0;
	double dac2val = 0.0;
	double dac3val = 0.0;
	double dac4val = 0.0;
	dac1.set(dac1val);
	dac2.set(dac2val);
	dac3.set(dac3val);
	dac4.set(dac4val);
	
	auto &enc1 = *hal.getRealPeripheralInput("enc1");
	auto &enc2 = *hal.getRealPeripheralInput("enc2");
	auto &enc3 = *hal.getRealPeripheralInput("enc3");
	auto &enc4 = *hal.getRealPeripheralInput("enc4");
	
	auto &enableDrv = *hal.getLogicPeripheralOutput("enableDrv");
	auto &readySig1 = *hal.getLogicPeripheralOutput("readySig1");
	auto &readySig2 = *hal.getLogicPeripheralOutput("readySig2");
	auto &readySig3 = *hal.getLogicPeripheralOutput("readySig3");
	auto &readySig4 = *hal.getLogicPeripheralOutput("readySig4");
	
	auto &io0 = *hal.getLogicPeripheralOutput("io0");
	auto &io1 = *hal.getLogicPeripheralOutput("io1");
	auto &io2 = *hal.getLogicPeripheralOutput("io2");
	auto &io3 = *hal.getLogicPeripheralOutput("io3");
	auto &io4 = *hal.getLogicPeripheralOutput("io4");
	auto &io5 = *hal.getLogicPeripheralOutput("io5");
	
	auto &pwm0 = *hal.getRealPeripheralOutput("pwm0");
	auto &pwm1 = *hal.getRealPeripheralOutput("pwm1");
	auto &pwm2 = *hal.getRealPeripheralOutput("pwm2");
	auto &pwm3 = *hal.getRealPeripheralOutput("pwm3");
	
	pwm0.set(0.05);
	pwm1.set(0.1);
	pwm2.set(0.5);
	pwm3.set(0.7);
	
	std::cout << "gpio test reserve io, enable, ready" << std::endl;
	while(true){
		for(int i = 0; i < 10000; i++){
			//toggle digital outputs
			io0.set(!io0.get());
			enableDrv.set(!enableDrv.get());
			if(i%2 == 0){
			  io1.set(!io1.get());
			  readySig1.set(!readySig1.get());
			}
			if(i%3 == 0){
			  io2.set(!io2.get());
			  readySig2.set(!readySig2.get());
			}
			if(i%4 == 0){
			  io3.set(!io3.get());
			  readySig3.set(!readySig3.get());
			}
			if(i%5 == 0){
			  io4.set(!io4.get());
			  readySig4.set(!readySig4.get());
			}
			if(i%6 == 0){
			  io5.set(!io5.get());
			}
			usleep(1000);
		}
	}
	
	/*std::cout << "gpio test ready, enable" << std::endl;
	for(int i = 0; i < 2000; i++){
		//toggle digital outputs
		enableDrv.set(!enableDrv.get());
		if(i%2 == 0){
		    readySig1.set(!readySig1.get());
		}
		if(i%3 == 0){
		    readySig2.set(!readySig2.get());			
		}
		if(i%4 == 0){
		    readySig3.set(!readySig3.get());
		}
		if(i%5 == 0){
		    readySig4.set(!readySig4.get());
		}
		usleep(10000);
	}*/
	/*
	std::cout << "encoder test:" << std::endl;
	std::cout << "enc1    enc2    enc3    enc4" << std::endl;
	
	//Toggle ready signals output values 
	for(int i = 0; i < 200; i++){
		// increase value of analog output
		dac1.set(dac1val += 0.5);
		if(dac1val > 10.0){
		  dac1val = -10.0;
		}
		dac2.set(dac2val += 1);
		if(dac2val > 10.0){
		  dac2val = -10.0;
		}
		dac3.set(dac3val += 0.4);
		if(dac3val > 10.0){
		  dac3val = -10.0;
		}
		dac4.set(dac4val += 0.2);
		if(dac4val > 10.0){
		  dac4val = -10.0;
		}
		
		//read enc values
		std::cout << enc1.get() << "    " << enc2.get() << "    ";
		std::cout << enc3.get() << "    " << enc4.get() << std::endl;
	  
		usleep(250000);
	}
	
	//Test of DAC
	std::cout << "DAC Test:" << std::endl;
	for(int i = 0; i < 100; i++){
		// increase value of analog output
		dac1.set(dac1val += 0.5);
		if(dac1val > 10.0){
		  dac1val = -10.0;
		}
		dac2.set(dac2val += 1);
		if(dac2val > 10.0){
		  dac2val = -10.0;
		}
		dac3.set(dac3val += 0.4);
		if(dac3val > 10.0){
		  dac3val = -10.0;
		}
		dac4.set(dac4val += 0.2);
		if(dac4val > 10.0){
		  dac4val = -10.0;
		}
		usleep(250000);
	}
	*/
	std::cout << "Hardware demo finished..." << std::endl;
}
