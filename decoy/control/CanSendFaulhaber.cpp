#include "CanSendFaulhaber.hpp"
#include <iostream>

using namespace eeros::control;

CanSendFaulhaber::CanSendFaulhaber(int sock, std::initializer_list<int> node): sock(sock),nodes(node), lastPosValue(node.size(), 0) {
//  std::cout << "send constructed " << node.size() << " nodes list" << std::endl;
// 	std::cout << "send node numbers: " << std::endl;
	for(int i = 0; i < node.size(); i++){
		std::cout << nodes[i] << std::endl;
		in.push_back(new Input<int>()); 
	}
	pdoRequested.getSignal().setValue(false);
	std::cout << "canSend:" << std::endl;
	for(int i = 0; i < lastPosValue.size(); i++){
		std::cout << i << ": " << lastPosValue[i] << std::endl;
	}
}

CanSendFaulhaber::~CanSendFaulhaber() {
	nodes.clear();
	in.clear();
}

void CanSendFaulhaber::run() {
	
	if(enabled){
		for(int i = 0; i < in.size(); i++){
			if( in[i]->getSignal().getValue() != lastPosValue[i] ){
				err = 0;
				err = canopen_pdo_send_2bytes(sock, nodes[i], CANOPEN_FC_PDO1_RX, 0x000F);
				err = canopen_pdo_set_abs_position(sock, nodes[i], CANOPEN_FC_PDO2_RX, 0x003F, in[i]->getSignal().getValue());
				lastPosValue[i] = in[i]->getSignal().getValue();
				if(err != 0){
					throw eeros::EEROSException("set of position over CAN failed");
				}
			}
		}
		stateCnt++;
		if(stateCnt > 200){
			for(int i = 0; i < in.size(); i++){
				initiatePdoRequest(nodes[i], CANOPEN_FC_PDO1_TX);
			}
			stateCnt = 0;
		}
	}
}

Input<int>* CanSendFaulhaber::getInput(int node) {
	for(int i = 0; i < nodes.size(); i++){
		if(nodes[i] == node) {
			return in[i];
		}
	}
	
	throw eeros::EEROSException("Error: specified CAN node not found");

}

void CanSendFaulhaber::sendPdo(int node, uint8_t function_code, uint32_t userData){
	int err = canopen_pdo_send_2bytes(sock, node, function_code, userData);
	if(err != 0){
		throw eeros::EEROSException("send pdo failed");
	}
}

void CanSendFaulhaber::initiatePdoRequest(int node, uint8_t function_code)
{
	err = canopen_pdo_request(sock, node, function_code);
	if(err < 0){
		std::cout << "err: " << err << std::endl;
		throw eeros::EEROSException("failed to initiate pdo request");
		return;
	}
	pdoRequested.getSignal().setValue(true);
}

void CanSendFaulhaber::enable(){
	enabled = true;
}

void CanSendFaulhaber::disable(){
	enabled = false;
}