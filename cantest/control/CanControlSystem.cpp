#include "CanControlSystem.hpp"
#include <eeros/core/Executor.hpp>
#include <iostream>

using namespace eeros::control;

CanControlSystem::CanControlSystem(int canSock, double ts) :
	canSend(canSock, {0x05, 0x04}),
	canReceive(canSock, {0x05, 0x04}, {CANOPEN_FC_PDO1_TX, CANOPEN_FC_PDO2_RX, CANOPEN_FC_PDO2_TX}),
	setPos(),
	timedomain("Main time domain", ts, true)
{
	
	canSend.getInput(0x05)->connect(setPos.getOut());
	canReceive.getPdoSignalIn().connect(canSend.getPdoSignalOut());
	
	timedomain.addBlock(&canReceive);
	timedomain.addBlock(&setPos);
	timedomain.addBlock(&canSend);
	
	eeros::task::Periodic td("control system",ts, timedomain);
	eeros::Executor::instance().add(timedomain);
}

CanControlSystem::~CanControlSystem(){
	canReceive.~CanReceiveFaulhaber();
	canSend.~CanSendFaulhaber();
}

