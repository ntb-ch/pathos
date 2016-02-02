#include <pathos/Hardware.hpp>
#include <eeros/hal/HAL.hpp>
#include <pathos/constants.hpp>

using namespace pathos;

namespace {
	namespace Subdevice {
		enum {
			pwm = 0,
			dac = 2,
			fqd = 3,
			gpio_pwm = 4,
			ready_enable = 5
		};
	}
}

Hardware::Hardware() :
	device("/dev/flink0"),
	
	dac1("dac1", &device, Subdevice::dac, 0, dac1Scale, dac1Offset),
	dac2("dac2", &device, Subdevice::dac, 2, dac2Scale, dac2Offset),
	dac3("dac3", &device, Subdevice::dac, 4, dac3Scale, dac3Offset),
	dac4("dac4", &device, Subdevice::dac, 6, dac4Scale, dac4Offset),
	
	enableDrv("enableDrv", &device, Subdevice::ready_enable, 0),
	readySig1("readySig1", &device, Subdevice::ready_enable, 1),
	readySig2("readySig2", &device, Subdevice::ready_enable, 2),
	readySig3("readySig3", &device, Subdevice::ready_enable, 3),
	readySig4("readySig4", &device, Subdevice::ready_enable, 4),
	
	enc1("enc1", &device, Subdevice::fqd, 0, -enc1Scale, enc1Offset), 
	enc2("enc2", &device, Subdevice::fqd, 1, -enc2Scale, enc2Offset),
	enc3("enc3", &device, Subdevice::fqd, 2, -enc3Scale, enc3Offset),
	enc4("enc4", &device, Subdevice::fqd, 3, -enc4Scale, enc4Offset),
	
	io0("io0", &device, Subdevice::gpio_pwm, 0),
	io1("io1", &device, Subdevice::gpio_pwm, 1),
	io2("io2", &device, Subdevice::gpio_pwm, 2),
	io3("io3", &device, Subdevice::gpio_pwm, 3),
	io4("io4", &device, Subdevice::gpio_pwm, 4),
	io5("io5", &device, Subdevice::gpio_pwm, 5),
	
	pwm0("pwm0", &device, Subdevice::pwm, 0),
	pwm1("pwm1", &device, Subdevice::pwm, 1),
	pwm2("pwm2", &device, Subdevice::pwm, 2),
	pwm3("pwm3", &device, Subdevice::pwm, 3)
{
	using namespace eeros::hal;
	
	HAL& hal = HAL::instance();
	hal.addPeripheralOutput(&dac1);
	hal.addPeripheralOutput(&dac2);
	hal.addPeripheralOutput(&dac3);
	hal.addPeripheralOutput(&dac4);
	
	dac1.set(0.0);
	dac2.set(0.0);
	dac3.set(0.0);
	dac4.set(0.0);

	hal.addPeripheralOutput(&enableDrv);
	hal.addPeripheralInput(&readySig1);
	hal.addPeripheralInput(&readySig2);
	hal.addPeripheralInput(&readySig3);
	hal.addPeripheralInput(&readySig4);
	
	hal.addPeripheralInput(&enc1);
	hal.addPeripheralInput(&enc2);
	hal.addPeripheralInput(&enc3);
	hal.addPeripheralInput(&enc4);
	
	hal.addPeripheralOutput(&io0);
	hal.addPeripheralOutput(&io1);
	hal.addPeripheralOutput(&io2);
	hal.addPeripheralOutput(&io3);
	hal.addPeripheralOutput(&io4);
	hal.addPeripheralOutput(&io5);
	
	hal.addPeripheralOutput(&pwm0);
	hal.addPeripheralOutput(&pwm1);
	hal.addPeripheralOutput(&pwm2);
	hal.addPeripheralOutput(&pwm3);
	pwm0.setFrequency(100);
	pwm1.setFrequency(100);
	pwm2.setFrequency(100);
	pwm3.setFrequency(100);
}
