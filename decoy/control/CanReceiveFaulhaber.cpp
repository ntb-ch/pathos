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
		axisEnabled.push_back(false);
		std::map<uint32_t, PdoUserData> m1;
		fcOut.push_back(new Output<uint8_t>());
		drvCtrlOut.push_back(new Output<uint16_t>());
		userDataOut.push_back(new Output<int32_t>());
		for(int j = 0; j < functionCode.size(); j++){
			m1[functionCodes[j]] = {functionCodes[j],static_cast<int16_t>(0),static_cast<int32_t>(0)};
		}
		nodePdoData.push_back(m1);
	}
	std::cout << "fc size: " << functionCode.size() << std::endl;
	
	for(int j = 0; j < nodePdoData.size(); j++){
		std::cout << "map"<< j << " of node " << nodes[j] << std::endl;
		for(auto p = nodePdoData[j].begin(); p != nodePdoData[j].end(); ++p){
			std::cout << p->first << "\t" << (p->second.functionCode)+0x30-48 << "\t" << p->second.drvCtrl << "\t" << p->second.userData << std::endl;
		}
	}
}

CanReceiveFaulhaber::~CanReceiveFaulhaber(){
	nodes.clear();
	fcOut.clear();
	drvCtrlOut.clear();
	userDataOut.clear();
	functionCodes.clear();
	nodePdoData.clear();
	
}

void CanReceiveFaulhaber::run()
{
	if(allAxisEnabled){
// 		debCnt++;
// 		if(debCnt > 200){
// 			for(int j = 0; j < nodePdoData.size(); j++){
// 				std::cout << "map"<< j << " of node " << nodes[j] << std::endl;
// 				for(auto p = nodePdoData[j].begin(); p != nodePdoData[j].end(); p++){
// 					std::cout << p->first << "\t" << (p->second.functionCode)+0x30-48 << "\t" <<std::hex << p->second.drvCtrl << std::dec<< "\t" << static_cast<int32_t>(p->second.userData) << std::endl;
// 				}
// 			}
// 			debCnt = 0;
// 		}
		if(pdoRequested.getSignal().getValue() == true){
			readLen = 0;
			readLen = canopen_read_frame(sock, &readFrame);
			if(readLen == EAGAIN || readLen == EWOULDBLOCK){
// 				std::cout << "would block" << std::endl;
// 				throw eeros::EEROSException("socket would block");
			}
			if(readLen > 0){
				for(int i = 0; i < nodes.size(); i++){
					if(readFrame.id == nodes[i]){		// check if frame.id (node) is registered
// 						std::cout << "nid: " << nodes[i] << " at pos " << i << std::endl;
// 						std::cout << "fc: " << readFrame.function_code << std::endl;
// 						if( pdoData.find(readFrame.function_code) != pdoData.end()){
						mIt = nodePdoData[i].find(readFrame.function_code);
						if( mIt != nodePdoData[i].end()){
							uint32_t tmpUd = 0; 
							if(readFrame.data_len > 2){
								tmpUd = (readFrame.payload.data[2] & 0x00FF);
								tmpUd |= ((readFrame.payload.data[3] & 0x00FF) << 8);
// 								pdoData[readFrame.function_code].userData = readFrame.payload.data[2] & 0x00FF;
// 								pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[3] & 0x00FF) << 8);
								mIt->second.userData = tmpUd;
								
								if(readFrame.data_len >= 4){
									tmpUd |= ((readFrame.payload.data[4] & 0x00FF) << 16);
									tmpUd |= ((readFrame.payload.data[5] & 0x00FF) << 24);
// 									pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[4] & 0x00FF) << 16);
// 									pdoData[readFrame.function_code].userData |= ((readFrame.payload.data[5] & 0x00FF) << 24);
									mIt->second.userData |= tmpUd;
								}
							}
							uint16_t tmpDrvCtrl = 0;
							tmpDrvCtrl = (readFrame.payload.data[0] & 0x00FF);
							tmpDrvCtrl |= ((readFrame.payload.data[1] & 0x00FF) << 8);
// 							pdoData[readFrame.function_code].drvCtrl = readFrame.payload.data[0] & 0x00FF;
// 							pdoData[readFrame.function_code].drvCtrl |= ((readFrame.payload.data[1] & 0x00FF) << 8);
							mIt->second.drvCtrl = tmpDrvCtrl;
							if(readFrame.function_code == CANOPEN_FC_PDO1_TX){
// 								std::cout << "3: " ;
								drvCtrlOut[i]->getSignal().setValue(tmpDrvCtrl);
							}
							else if(readFrame.function_code == CANOPEN_FC_PDO2_RX){
// 								std::cout << "6: " ;
							}
							else if(readFrame.function_code == CANOPEN_FC_PDO2_TX){
// 								std::cout << "5: " ;
								userDataOut[i]->getSignal().setValue(static_cast<int32_t>(tmpUd));
							}
// 							if(readFrame.id == 0x04){
// 							std::cout << "node: " << nodes[i] << "\t fc: " << functionCodes[i]+0x30-48 << "\t status: " << std::hex << tmpDrvCtrl << "\t data: " << std::dec << tmpUd << std::endl;
							
// 							std::cout << "node: " << nodes[i] << "\t fc: " << readFrame.function_code << "\t status: " << std::hex << mIt->second.drvCtrl << "\t data: " << std::dec << mIt->second.userData << std::endl;
// 							}
							pdoRequested.getSignal().setValue(false);
							fcOut[i]->getSignal().setValue(readFrame.function_code);
							
							
							if( (tmpDrvCtrl & 0x002F) == 0x0027){	//check if operation is enabled
// 								axisEnabled[i] = true;
// 								for(int k = 0; k < nodes.size(); k++){
// 									allAxisEnabled = axisEnabled[i] && axisEnabled[i-1];
// 								}
// 								if(allAxisEnabled) {
									isEnabled.getSignal().setValue(true);
// 								}
							}
							else{
// 								axisEnabled[i] = false;
// 								allAxisEnabled = false;
								isEnabled.getSignal().setValue(false);
							}
						}
					}
				}
			}
		}
	}
}

/*int CanReceiveFaulhaber::getPdoValue(uint32_t nodeId, uint8_t functionCode, uint16_t* drvCtrl, uint32_t* userData) {
	auto it = std::find(nodes.begin(), nodes.end(), nodeId);
	int i = std::distance(nodes.begin(),it);
	if( it != nodes.end() ){
		if((mIt = nodePdoData[i].find(functionCode)) != nodePdoData[i].end()){
			*drvCtrl = mIt->second.drvCtrl;
			*userData = mIt->second.userData;
			return 0;
		}
		throw eeros::EEROSException("getPdoValue: no such function code");
	}
	throw eeros::EEROSException("getPdoValue: no such node");
	return -1;
}*/

void CanReceiveFaulhaber::enable(){
	allAxisEnabled = true;
}

void CanReceiveFaulhaber::disable(){
	allAxisEnabled = false;
}

Output<bool> CanReceiveFaulhaber::getOpEnabledOut(){
	return isEnabled;
}

Output<uint8_t>& CanReceiveFaulhaber::getFcOut(uint32_t node){
	for(int i = 0; i < nodes.size(); i++){
		if(node == nodes[i]){
			return (*fcOut[i]);
		}
	}
}

Output<uint16_t>& CanReceiveFaulhaber::getdrvCtrlOut(uint32_t node){
	for(int i = 0; i < nodes.size(); i++){
		if(node == nodes[i]){
			return (*drvCtrlOut[i]);
		}
	}
}

Output<int32_t>& CanReceiveFaulhaber::getUserDataOut(uint32_t node){
	for(int i = 0; i < nodes.size(); i++){
		if(node == nodes[i]){
			return (*userDataOut[i]);
		}
	}
}
