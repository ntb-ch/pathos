#include "ControlSystem_decoy.hpp"
#include <eeros/core/Executor.hpp>
#include <iostream>

#include "../constants.hpp"

using namespace eeros::control;
using namespace pathos::decoy;

ControlSystem_decoy::ControlSystem_decoy(int canSock, double ts) :
	canSend(canSock, {node_armLeft, node_armRight}),
	canReceive(canSock, {node_armLeft, node_armRight}, {CANOPEN_FC_PDO1_TX, CANOPEN_FC_PDO2_RX, CANOPEN_FC_PDO2_TX}),
	setPosRad_toPulses(arm_encPulse, arm_i),
	timedomain("Main time domain", ts, true)
{
	setPosRad_toPulses.getIn().connect(setPosRad.getOut());
	canSend.getInput(node_armRight)->connect(setPosRad_toPulses.getOut());       // I send you a position setpoint
	canReceive.getPdoSignalIn().connect(canSend.getPdoSignalOut());              // I get in an encoder position ?? NO.. TODO
	
	timedomain.addBlock(&canReceive);
	timedomain.addBlock(&setPosRad);
	timedomain.addBlock(&setPosRad_toPulses);
	timedomain.addBlock(&canSend);
	
	eeros::task::Periodic td("control system",ts, timedomain);
	eeros::Executor::instance().add(timedomain);
}

ControlSystem_decoy::~ControlSystem_decoy(){
	canReceive.~CanReceiveFaulhaber();
	canSend.~CanSendFaulhaber();
}

int ControlSystem_decoy::getActualPos_pulses(){
	int16_t drvCtrl = 0;
	int32_t encPos = 0;
	
	canSend.initiatePdoRequest(node_armRight, CANOPEN_FC_PDO2_TX);
	if(canReceive.getPdoValue(node_armRight, CANOPEN_FC_PDO2_TX, &drvCtrl, &encPos)!=0){  
		throw eeros::EEROSException("function code not found");
	}
	else{
		return encPos;
	}
}

double ControlSystem_decoy::getActualPos_rad(){
	int16_t drvCtrl = 0;
	int32_t encPos = 0;
	
	canSend.initiatePdoRequest(node_armRight, CANOPEN_FC_PDO2_TX);
	if(canReceive.getPdoValue(node_armRight, CANOPEN_FC_PDO2_TX, &drvCtrl, &encPos)!=0){  
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
	
	canSend.initiatePdoRequest(node_armRight, CANOPEN_FC_PDO1_TX);                    
	if(canReceive.getPdoValue(node_armRight, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
		throw eeros::EEROSException("function code not found");
	}
	else{
		// Wait until drive is in "operation enabled" mode
		if( (drvCtrl & oneBitMask) == operationEnabled ){
			return true;
		}
		else
			return false;
	}
}

// CANOPEN_FC_PDO1_TX = get status word from motor
// CANOPEN_FC_PDO2_RX = send data to motor (reference position)
// CANOPEN_FC_PDO2_TX = get Data from motor
