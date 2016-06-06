#include "ControlSystem_decoy.hpp"
#include <eeros/core/Executor.hpp>
#include <iostream>

#include "../constants.hpp"

using namespace eeros::control;
using namespace pathos::decoy;

ControlSystem_decoy::ControlSystem_decoy(int canSock, double ts) :
	setPosRad_armLeft(0.0),
	setPosRad_armRight(0.0),
	socket(canSock),
	canSend(canSock, {node_turning, node_swingBack, node_swingFront, node_armLeft, node_armRight}),
	canReceive(canSock, {node_turning, node_swingBack, node_swingFront, node_armLeft, node_armRight}, {CANOPEN_FC_PDO1_TX, CANOPEN_FC_PDO2_RX, CANOPEN_FC_PDO2_TX}),
	radToPulses_al(arm_encPulse, arm_i),
	radToPulses_ar(arm_encPulse, arm_i),
	timedomain("Main time domain", ts, true)
{
	// Connect blocks
	radToPulses_al.getIn().connect(setPosRad_armLeft.getOut());
	radToPulses_ar.getIn().connect(setPosRad_armRight.getOut());
	
	canSend.getInput(node_armRight)->connect(radToPulses_ar.getOut()); // send a position setpoint
	canSend.getInput(node_armLeft)->connect(radToPulses_al.getOut());  // send a position setpoint
	
	canReceive.getPdoSignalIn().connect(canSend.getPdoSignalOut());   
	
	// Add to timedomain
	timedomain.addBlock(&canReceive);
	
	timedomain.addBlock(&setPosRad_armLeft);
	timedomain.addBlock(&setPosRad_armRight);
	timedomain.addBlock(&radToPulses_al);
	timedomain.addBlock(&radToPulses_ar);
	
	timedomain.addBlock(&canSend);
	
	eeros::task::Periodic td("control system",ts, timedomain);
	eeros::Executor::instance().add(timedomain);
}

ControlSystem_decoy::~ControlSystem_decoy(){
	canReceive.~CanReceiveFaulhaber();
	canSend.~CanSendFaulhaber();
}

int ControlSystem_decoy::getActualPos_pulses(int node){
	int16_t drvCtrl = 0;
	int32_t encPos = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO2_TX);
	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO2_TX, &drvCtrl, &encPos)!=0){  
		throw eeros::EEROSException("function code not found");
	}
	else{
		return encPos;
		
	}
}

double ControlSystem_decoy::getActualPos_rad(int node){
	int16_t drvCtrl = 0;
	int32_t encPos = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO2_TX);
	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO2_TX, &drvCtrl, &encPos)!=0){  
		throw eeros::EEROSException("function code not found");
	}
	else{
		double out = encPos * (2.0*pi) / arm_encPulse / arm_i;
		return out;
	}
}

bool ControlSystem_decoy::isOperationEnabled(){
	int16_t drvCtrl = 0;
	int32_t data = 0;
	
	static bool allAxisEnabled = false;
	static bool isEnabled[nofAxis];
	for(int i=0;i<nofAxis;i++) isEnabled[i] = false;
	
	for(int i=0; i<nofAxis; i++){
		canSend.initiatePdoRequest(nodes[i], CANOPEN_FC_PDO1_TX);                    
		if(canReceive.getPdoValue(nodes[i], CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
			throw eeros::EEROSException("function code not found");
		}
		else{
			// Wait until drive is in "operation enabled" mode
			if( (drvCtrl & oneBitMask) == operationEnabled ){
				isEnabled[i] = true;
			}
			else {
				isEnabled[i] = false;
			}
		}
	}
	
	// Check if all axes are enabled
	for(int i=1; i<nofAxis; i++){
		allAxisEnabled = isEnabled[i] && isEnabled[i-1];
	}
	return allAxisEnabled;
}

bool ControlSystem_decoy::isHomed(){ // TODO for all axes
	int16_t drvCtrl = 0;
	int32_t data = 0;
	
	canSend.initiatePdoRequest(node_armLeft, CANOPEN_FC_PDO1_TX);  // get status word
	if(canReceive.getPdoValue(node_armLeft, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
		throw eeros::EEROSException("function code not found");
		}
	else{
		int16_t drv = drvCtrl;
		int16_t bit = (1<<12);
		int16_t res  = drvCtrl & (1<<12);
		if((drvCtrl & bit12Mask)>0){
// 			std::cout << "homed: " << drv << "; " << bit << "; " << res << "; " << oe_homingDone << std::endl;
			return true;
		}
		else {
// 			std::cout << "not homed: " << drv << "; " << bit << "; " << res << "; " << oe_homingDone << std::endl;
			return false;
		}
	}
}

bool ControlSystem_decoy::isHomingError(){ // TODO for all axes
	int16_t drvCtrl = 0;
	int32_t data = 0;
	
	canSend.initiatePdoRequest(node_armLeft, CANOPEN_FC_PDO1_TX);  // get status word
	if(canReceive.getPdoValue(node_armLeft, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
		throw eeros::EEROSException("function code not found");
		}
	else{		
		int16_t drv = drvCtrl & oneBitMask;
		int16_t bit = (1<<13);
		int16_t res  = drv & (1<<13);
		if((drvCtrl & bit13Mask)>0){
// 			std::cout << "error: " << drv << "; " << bit << "; " << res << "; " << oe_homingError << std::endl;
			return true;
		}
		else {
// 			std::cout << "not error: " << drv << "; " << bit << "; " << res << "; " << oe_homingError << std::endl;
			return false;
		}
	}
}

int16_t ControlSystem_decoy::getStatusWord(int node){
	int16_t drvCtrl = 0;
	int32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX);                    
	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
			throw eeros::EEROSException("function code not found");
		}
	else
// 		return (drvCtrl & oneBitMask);
		return drvCtrl;
}

// CANOPEN_FC_PDO1_RX = control word 
// CANOPEN_FC_PDO1_TX = get status word from motor
// CANOPEN_FC_PDO2_RX = send data to motor (reference position)
// CANOPEN_FC_PDO2_TX = get Data from motor


// PDO1_RX (receive) controlword
// PDO1_TX (send) statusword

// PDO2_RX (receive) command
// PDO2_TX (send) abfragedaten (??)


