#include "CanReceiveFaulhaber.hpp"
#include <eeros/core/EEROSException.hpp>
#include <iostream>
#include <algorithm>
#include <unistd.h>

using namespace eeros::control;

CanReceiveFaulhaber::CanReceiveFaulhaber(int sock, std::initializer_list<uint32_t> node, std::initializer_list<uint8_t> functionCode) : sock(sock), nodes(node), functionCodes(functionCode) {
	std::cout << "rec constructed " << node.size() << " nodes list" << std::endl;
	std::cout << "rec node numbers: " << std::endl;
	for(int i = 0; i < node.size(); i++){
		std::cout << nodes[i] << std::endl;
		out.push_back(new Output<int>());
	}
	for(int i = 0; i < functionCode.size(); i++){
		pdoData[functionCodes[i]] = {0,0};
		std::cout << "fc: " << functionCodes[i] << "\t userData: " << pdoData[functionCodes[i]].drvCtrl << "\t" << pdoData[functionCodes[i]].userData << std::endl;
	}
	std::cout << "map"<< std::endl;
	for(auto p = pdoData.begin(); p != pdoData.end(); ++p){
		std::cout << p->first << "\t" << p->second.drvCtrl << "\t" << p->second.userData << std::endl;
	}
}

CanReceiveFaulhaber::~CanReceiveFaulhaber(){
	nodes.clear();
	out.clear();
	functionCodes.clear();
	pdoData.clear();
	
}

void CanReceiveFaulhaber::run()
{
	if(pdoRequested.getSignal().getValue() == true){
		readLen = 0;
		readLen = canopen_read_frame(sock, &readFrame);
		if(readLen == EAGAIN || readLen == EWOULDBLOCK){
			std::cout << "would block" << std::endl;
		}
		if(readLen > 0){
			for(int i = 0; i < nodes.size(); i++){
				if(readFrame.id == nodes[i]){		// check if frame.id (node) is registered
					if( pdoData.find(readFrame.function_code) != pdoData.end()){
						if(readFrame.data_len > 2){
							pdoData[readFrame.function_code].userData = readFrame.payload.data[2] & 0x00FF;
							pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[3] & 0x00FF) << 8);
							if(readFrame.data_len >= 4){
								pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[4] & 0x00FF) << 16);
								pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[5] & 0x00FF) << 24);
							}
						}
						pdoData[readFrame.function_code].drvCtrl = readFrame.payload.data[0] & 0x00FF;
						pdoData[readFrame.function_code].drvCtrl |= ((readFrame.payload.data[1] & 0x00FF) << 8);
						pdoData[readFrame.function_code].nodeId = readFrame.id;
// 						std::cout << "node: " << nodes[i] << "\t fc: " << functionCodes[i] << "\t status: " << std::hex << pdoData[functionCodes[i]].drvCtrl << "\t data: " << std::dec << pdoData[functionCodes[i]].userData << std::endl;
						pdoRequested.getSignal().setValue(false);
					}
				}
			}
		}
	}
}

int CanReceiveFaulhaber::getPdoValue(int32_t nodeId, uint8_t functionCode, int16_t* drvCtrl, int32_t* userData) {
	auto it = std::find(nodes.begin(), nodes.end(), nodeId);
	if( it != nodes.end() ){
		if (pdoData.find(CANOPEN_FC_PDO2_TX) != pdoData.end()) {
			*drvCtrl = pdoData[functionCode].drvCtrl;
			*userData = pdoData[functionCode].userData;
			return 0;
		}
	}
	throw eeros::EEROSException("getPdoValue: no such node");
	return -1;
}
