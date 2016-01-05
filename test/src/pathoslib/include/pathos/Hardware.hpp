#ifndef PATHOS_HARDWARE_HPP_
#define PATHOS_HARDWARE_HPP_

#include <eeros/hal/FlinkDevice.hpp>
#include <eeros/hal/FlinkAnalogOut.hpp>
#include <eeros/hal/FlinkFqd.hpp>
#include <eeros/hal/FlinkDigOut.hpp>
#include <eeros/hal/FlinkDigIn.hpp>
#include <eeros/hal/FlinkPwm.hpp>

namespace pathos {
	
    class Hardware {
    public:
		Hardware();
    private:
		eeros::hal::FlinkDevice device;
		
		eeros::hal::FlinkAnalogOut dac1;
		eeros::hal::FlinkAnalogOut dac2;
		eeros::hal::FlinkAnalogOut dac3;
		eeros::hal::FlinkAnalogOut dac4;
		
		eeros::hal::FlinkDigOut enableDrv;
		eeros::hal::FlinkDigOut readySig1;
		eeros::hal::FlinkDigOut readySig2;
		eeros::hal::FlinkDigOut readySig3;
		eeros::hal::FlinkDigOut readySig4;
		
		eeros::hal::FlinkFqd enc1;
		eeros::hal::FlinkFqd enc2;
		eeros::hal::FlinkFqd enc3;
		eeros::hal::FlinkFqd enc4;
		
		eeros::hal::FlinkDigOut io0;
		eeros::hal::FlinkDigOut io1;
		eeros::hal::FlinkDigOut io2;
		eeros::hal::FlinkDigOut io3;
		eeros::hal::FlinkDigOut io4;
		eeros::hal::FlinkDigOut io5;
		
		eeros::hal::FlinkPwm pwm0;
		eeros::hal::FlinkPwm pwm1;
		eeros::hal::FlinkPwm pwm2;
		eeros::hal::FlinkPwm pwm3;
    };
}

/*
 * Signal Names:
 * *************
 * 
 * 
 * 
 */

#endif // PATHOS_HARDWARE_HPP_
