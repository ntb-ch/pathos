#include "ReadSensorData.hpp"
#include <eeros/core/System.hpp>

#include <iostream>

using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros;

ReadSensorData::ReadSensorData(SensorsThread* sensThread, std::array<int,60> sensAreas) : 
sensorsThread(sensThread), 
sensorsAreas(sensAreas) 

{ 
	std::array<int,15> tmp_visible, tmp_close, tmp_near, tmp_far;
	
	int i = 0;
	while(i < 60){
		if(i<15)
			tmp_visible[i]  = sensAreas[i]; 
		else if (i < 30)
			tmp_close[i-15] = sensAreas[i]; 
		else if (i < 45)
			tmp_near[i-30]  = sensAreas[i];
		else if (i < 60)
			tmp_far[i-45]   = sensAreas[i];
		i++;
	}
	
	id_visible = defineCells(tmp_visible);
	id_close   = defineCells(tmp_close);
	id_near    = defineCells(tmp_near);
	id_far     = defineCells(tmp_far);
	for(int i = 0; i < 48; i++) id_all.push_back(i);
	
	for(int i = 0; i < id_visible.size(); i++) std::cout << id_visible[i] << "; " ; std::cout << std::endl;
}

Output<int>& ReadSensorData::getOutStatus() {
	return status;
}

Output<int>& ReadSensorData::getOutPrevStatus() {
	return prevStatus;
}

void ReadSensorData::run() {
	timestamp_t time = System::getTimeNs();
	int tmp_status, tmp_prev_status; 
	
	// get bool signal from 48 sensors cells
	std::array<bool,NUM_ID> sensors_data = sensorsThread->getBuffer();
	
	sbdFcn1 = isSomebody(sensors_data, id_visible);
	sbdFcn2 = isSomebody(sensors_data, id_close);
	sbdFcn3 = isSomebody(sensors_data, id_near);
	sbdFcn4 = isSomebody(sensors_data, id_far);
	sbdFcn5 = isSomebody(sensors_data, id_all);
	
	if(sbdFcn1)      tmp_status = visible;
	else if(sbdFcn2) tmp_status = close;
	else if(sbdFcn3) tmp_status = near;
	else if(sbdFcn4) tmp_status = far;
	else if(sbdFcn5) tmp_status = somebody;  // somebody in the room, not in my interest field
	else             tmp_status = nobody;    // nobody in the room
	
	status.getSignal().setValue(tmp_status);
	prevStatus.getSignal().setValue(tmp_prev_status);  
	
	// set timestamp
	status.getSignal().setTimestamp(time);
	prevStatus.getSignal().setTimestamp(time);
	
	tmp_prev_status = status.getSignal().getValue();
}

std::vector<int> ReadSensorData::defineCells(std::array<int,15> data) {
	std::vector<int> out;
	int i = 0;
	
	for(i = 0; i < 15; i++){
		if (data[i] == -1)
			return out;
		else 
			out.push_back(data[i]);
	}
	return out;
}

int ReadSensorData::isSomebody(std::array<bool,NUM_ID> sensors_data, std::vector<int> data) {
	bool out = false;
	
	for (int i = 0; i < data.size(); i++){
		int index = data[i];
		if (sensors_data[index] == true){
			out = true;
			break;
		}
	}
	return out;
}
