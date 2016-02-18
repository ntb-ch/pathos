#include "ControlSystem_Teach.hpp"
#include <eeros/hal/HAL.hpp>
#include "../constants.hpp"
#include <iostream>
 
using namespace eeros::hal;
using namespace pathos::peepingpanel;
 
ControlSystem_Teach::ControlSystem_Teach(std::string enc_id, std::string dac_id) :

enc(enc_id),
pathPlanner(velMax, accMax, decMax, dt),
i_ref(i_gear),
kp(pos_ctrl_gain),
kv(vel_ctrl_gain),
speedInit(0.0),
speedSwitch(0),
inertia(J_motor),
inv_km(1.0/km),
dac_saturation(10.0),
dacConst(0.0),
dacSwitch(0),
dac(dac_id),
timedomain("Main time domain", dt, true)

{
	HAL& hal = HAL::instance();
	
    // Configure Blocks
	sumPos.negateInput(1);
	sumVel.negateInput(1);
	speedSwitch.switchToInput(0);
	dacSwitch.switchToInput(0);
	dac_saturation.enable();
	
    // Connect Blocks
	mux_enc.getIn(0).connect(enc.getOut());
	sum_enc_offset.getIn(0).connect(mux_enc.getOut());
	sum_enc_offset.getIn(1).connect(enc_offset.getOut());
	encSpeed.getIn().connect(sum_enc_offset.getOut());
	i_ref.getIn().connect(pathPlanner.getPosOut());
	refSpeed.getIn().connect(i_ref.getOut());
	sumPos.getIn(0).connect(i_ref.getOut());
	sumPos.getIn(1).connect(sum_enc_offset.getOut());
	kp.getIn().connect(sumPos.getOut());
	sumVelRef.getIn(0).connect(kp.getOut());
	sumVelRef.getIn(1).connect(refSpeed.getOut());
	speedSwitch.getIn(0).connect(speedInit.getOut());
	speedSwitch.getIn(1).connect(sumVelRef.getOut());
	sumVel.getIn(0).connect(speedSwitch.getOut());
	sumVel.getIn(1).connect(encSpeed.getOut());
	kv.getIn().connect(sumVel.getOut());
	inertia.getIn().connect(kv.getOut());
	inv_km.getIn().connect(inertia.getOut());
	demux_dac.getIn().connect(inv_km.getOut());
	
	dacSwitch.getIn(0).connect(dacConst.getOut());
	dacSwitch.getIn(1).connect(demux_dac.getOut(0));
	dac_saturation.getIn().connect(dacSwitch.getOut());
	dac.getIn().connect(dac_saturation.getOut());

    // Run Blocks	
	timedomain.addBlock(&enc);
	timedomain.addBlock(&mux_enc);
	timedomain.addBlock(&enc_offset);
	timedomain.addBlock(&sum_enc_offset);
	timedomain.addBlock(&pathPlanner);
	timedomain.addBlock(&i_ref);
	timedomain.addBlock(&sumPos);
	timedomain.addBlock(&refSpeed);
	timedomain.addBlock(&kp);
	timedomain.addBlock(&sumVelRef);
	timedomain.addBlock(&encSpeed);
	timedomain.addBlock(&speedInit);
	timedomain.addBlock(&speedSwitch);
	timedomain.addBlock(&sumVel);
	timedomain.addBlock(&kv);
	timedomain.addBlock(&inertia);
	timedomain.addBlock(&inv_km);
	timedomain.addBlock(&demux_dac);
	timedomain.addBlock(&dacConst);
	timedomain.addBlock(&dacSwitch);
	timedomain.addBlock(&dac_saturation);
	timedomain.addBlock(&dac);	
}
 
void ControlSystem_Teach::start() {
    timedomain.start();
}
 
void ControlSystem_Teach::stop() {
    timedomain.stop();
    timedomain.join();  
}