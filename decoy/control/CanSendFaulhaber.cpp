#include "CanSendFaulhaber.hpp"
#include <iostream>

using namespace eeros::control;

CanSendFaulhaber::CanSendFaulhaber(int sock, std::initializer_list<int> node): sock(sock),nodes(node), lastPosValue(node.size(), 0) {
 	std::cout << "send constructed " << node.size() << " nodes list" << std::endl;
	std::cout << "send node numbers: " << std::endl;
	for(int i = 0; i < node.size(); i++){
		std::cout << nodes[i] << std::endl;
		in.push_back(new Input<int>()); 
	}
	pdoRequested.getSignal().setValue(false);
}

CanSendFaulhaber::~CanSendFaulhaber() {
	nodes.clear();
	in.clear();
}

void CanSendFaulhaber::run() {
	
	for(int i = 0; i < in.size(); i++){
		if( in[i]->getSignal().getValue() != lastPosValue[i] ){
			err = 0;
			err = canopen_pdo_send_2bytes(sock, nodes[i], CANOPEN_FC_PDO1_RX, 0x000F);
			err = canopen_pdo_set_abs_position(sock, 0x05, CANOPEN_FC_PDO2_RX, 0x003F, in[i]->getSignal().getValue());
			lastPosValue[i] = in[i]->getSignal().getValue();
			if(err < 0){
				throw eeros::EEROSException("set of position over CAN failed");
			}
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

void CanSendFaulhaber::initiatePdoRequest(int node, uint8_t function_code)
{
	err = canopen_pdo_request(sock, node, function_code);
	if(err < 0){
		throw eeros::EEROSException("failed to initiate pdo request");
		return;
	}
	pdoRequested.getSignal().setValue(true);
}

