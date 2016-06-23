#include "ControlSystem_decoy.hpp"
#include <eeros/core/Executor.hpp>
#include <iostream>

#include "../constants.hpp"

using namespace eeros::control;
using namespace pathos::decoy;

ControlSystem_decoy::ControlSystem_decoy(int canSock, double ts) :
	setPosPulses_node1(0.0),
	setPosPulses_node2(0.0),
	setPosPulses_node3(0.0),
	setPosPulses_node4(0.0),
	setPosPulses_node5(0.0),
	
	socket(canSock),
	canSend(canSock, {node1, node2, node3, node4, node5}),
	canReceive(canSock, {node1, node2, node3, node4, node5}, {CANOPEN_FC_PDO1_TX, CANOPEN_FC_PDO2_RX, CANOPEN_FC_PDO2_TX}),
	
	timedomain("Main time domain", ts, true)
{
	// Connect blocks
// 	radToPulses_node4.getIn().connect(setPosRad_node4.getOut());
// 	radToPulses_node5.getIn().connect(setPosRad_node5.getOut());
	
// 	canSend.getInput(node4)->connect(radToPulses_node4.getOut());  // send a position setpoint
// 	canSend.getInput(node5)->connect(radToPulses_node5.getOut());  // send a position setpoint
	
	canSend.getInput(node1)->connect(setPosPulses_node1.getOut());  // send a position setpoint
	canSend.getInput(node2)->connect(setPosPulses_node2.getOut());  // send a position setpoint
	canSend.getInput(node3)->connect(setPosPulses_node3.getOut());  // send a position setpoint
	canSend.getInput(node4)->connect(setPosPulses_node4.getOut());  // send a position setpoint
	canSend.getInput(node5)->connect(setPosPulses_node5.getOut());  // send a position setpoint
	
	
	canReceive.getPdoSignalIn().connect(canSend.getPdoSignalOut());   
	
	// Add to timedomain
	timedomain.addBlock(&canReceive);
	
	timedomain.addBlock(&setPosPulses_node1);
	timedomain.addBlock(&setPosPulses_node2);
	timedomain.addBlock(&setPosPulses_node3);
	timedomain.addBlock(&setPosPulses_node4);
	timedomain.addBlock(&setPosPulses_node5);
	
	timedomain.addBlock(&canSend);
	
	eeros::task::Periodic td("control system",ts, timedomain);
	eeros::Executor::instance().add(timedomain);
}

ControlSystem_decoy::~ControlSystem_decoy(){
	canReceive.~CanReceiveFaulhaber();
	canSend.~CanSendFaulhaber();
}

int ControlSystem_decoy::getActualPos_pulses(int node){
	uint16_t drvCtrl = 0;
	uint32_t encPos = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO2_TX);
	//TODO
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO2_TX, &drvCtrl, &encPos)!=0){
	encPos = canReceive.getUserDataOut(node).getSignal().getValue();
// 		throw eeros::EEROSException("function code not found");
// 	}
// 	else{
		return encPos;
		
// 	}
}

double ControlSystem_decoy::getActualPos_rad(int node){
	uint16_t drvCtrl = 0;
	uint32_t encPos = 0;
	
	getActualPos_pulses(node);
	
	int gear = 0;
	if(node==1)
		gear = turn_i * turn_i2;
	else if(node==2 || node==3)
		gear = swing_i;
	else
		gear = arm_i;
	
	
	double out = encPos * (2.0*pi) / arm_encPulse / gear;
	return out;
}

// bool ControlSystem_decoy::isOperationEnabled(){
// 	uint16_t drvCtrl = 0;
// 	uint32_t data = 0;
// 	
// 	static bool allAxisEnabled = false;
// // 	static bool isEnabled[nofAxis];TODO KALA
// 	static bool isEnabled = false;
// // 	for(int i=0;i<nofAxis;i++) isEnabled[i] = false; TODO KALA
// 	
// // 	TODO KALA
// // 	for(int i=0; i<nofAxis; i++){
// // 		canSend.initiatePdoRequest(nodes[i], CANOPEN_FC_PDO1_TX);                    
// // 		if(canReceive.getPdoValue(nodes[i], CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// // 			throw eeros::EEROSException("function code not found");
// // 		}
// // 		else{
// // 			// Wait until drive is in "operation enabled" mode
// // 			std::cout << nodes[i] << " st:" << std::hex << drvCtrl << std::dec << std::endl;
// // 			if( (drvCtrl & oneBitMask) == operationEnabled ){
// // 				isEnabled[i] = true;
// // 			}
// // 			else {
// // 				isEnabled[i] = false;
// // 			}
// // 		}
// // 	}
// 	
// // 	TODO KALA
// // 	for(int i=0; i<nofAxis; i++){
// 		canSend.initiatePdoRequest(0x04, CANOPEN_FC_PDO1_TX);                    
// 		if(canReceive.getPdoValue(0x04, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 			throw eeros::EEROSException("function code not found");
// 		}
// 		else{
// 			// Wait until drive is in "operation enabled" mode
// 			std::cout << 0x04 << " st:" << std::hex << drvCtrl << std::dec << std::endl;
// 			if( (drvCtrl & oneBitMask) == operationEnabled ){
// 				allAxisEnabled = true;
// 			}
// 			else {
// 				allAxisEnabled = false;
// 			}
// 		}
// // 	}
// // TODO KALA
// 	
// 	// Check if all axes are enabled
// // 	for(int i=1; i<nofAxis; i++){
// // 		allAxisEnabled = isEnabled[i] && isEnabled[i-1];
// // 	}
// // TODO KALA
// 	return allAxisEnabled;
// }

bool ControlSystem_decoy::isHomed(int node){
	uint16_t drvCtrl = 0;
	uint32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX);  
	drvCtrl = canReceive.getdrvCtrlOut(node).getSignal().getValue();
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 		throw eeros::EEROSException("function code not found");
// 	}
// 	else{
		if((drvCtrl & bit12Mask)>0)
			return true;
		else 
			return false;
// 	}
}

bool ControlSystem_decoy::isHomingError(int node){ 
	uint16_t drvCtrl = 0;
	uint32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX);
	drvCtrl = canReceive.getdrvCtrlOut(node).getSignal().getValue();
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 		throw eeros::EEROSException("function code not found");
// 		}
// 	else{		
		if((drvCtrl & bit13Mask)>0)
			return true;
		else 
			return false;
// 	}
}

uint16_t ControlSystem_decoy::getStatusWord(int node){
	uint16_t drvCtrl = 0;
	uint32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX);       
	drvCtrl = canReceive.getdrvCtrlOut(node).getSignal().getValue();
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 			throw eeros::EEROSException("function code not found");
// 		}
// 	else
// 		return (drvCtrl & oneBitMask);
		return drvCtrl;
}

bool ControlSystem_decoy::setPointReceived(int node){
	uint16_t drvCtrl = 0;
	uint32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX);  
	drvCtrl = canReceive.getdrvCtrlOut(node).getSignal().getValue();
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 		throw eeros::EEROSException("function code not found");
// 	}
// 	else{
		if((drvCtrl & bit12Mask)>0)
			return true;
		else 
			return false;
// 	}
}

bool ControlSystem_decoy::setPointReached(int node){
	uint16_t drvCtrl = 0;
	uint32_t data = 0;
	
	canSend.initiatePdoRequest(node, CANOPEN_FC_PDO1_TX); 
	drvCtrl = canReceive.getdrvCtrlOut(node).getSignal().getValue();
// 	if(canReceive.getPdoValue(node, CANOPEN_FC_PDO1_TX, &drvCtrl, &data)!=0){  
// 		throw eeros::EEROSException("function code not found");
// 	}
// 	else{
		if((drvCtrl & bit10Mask)>0)
			return true;
		else 
			return false;
// 	}
}


// CANOPEN_FC_PDO1_RX = control word 
// CANOPEN_FC_PDO1_TX = get status word from motor
// CANOPEN_FC_PDO2_RX = send data to motor (reference position)
// CANOPEN_FC_PDO2_TX = get Data from motor


// PDO1_RX (receive) controlword
// PDO1_TX (send) statusword
// PDO2_RX (receive) command
// PDO2_TX (send) abfragedaten (??)


