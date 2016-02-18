#include "ReadSensorData.hpp"
#include <eeros/core/System.hpp>

#include <iostream>

using namespace pathos::peepingpanel;
using namespace eeros::control;
using namespace eeros;

ReadSensorData::ReadSensorData(SensorsThread* sensorsdata, eeros::math::Matrix<4,15,int> sensAreas) : sensorsData(sensorsdata), sensorsAreas(sensAreas) { 
	id_visible = defineCells(sensorsAreas.getRow(0));
	id_close   = defineCells(sensorsAreas.getRow(1));
	id_near    = defineCells(sensorsAreas.getRow(2));
	id_far     = defineCells(sensorsAreas.getRow(3));
	for(int i = 0; i < 48; i++) id_all[i] = i;
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
	Array data = sensorsData->getBuffer();
	
	if(isSomebody(id_visible)) 
		tmp_status = visible;
	else if(isSomebody(id_close))
		tmp_status = close;
	else if(isSomebody(id_near))
		tmp_status = near;
	else if(isSomebody(id_far))
		tmp_status = far;
	else if(isSomebody(id_all))  // somebody in the room, not in my interest field
		tmp_status = somebody;
	else                          // nobody in the room
		tmp_status = nobody;
	
	status.getSignal().setValue(tmp_status);
	prevStatus.getSignal().setValue(tmp_prev_status);    
	
	status.getSignal().setTimestamp(time);
	prevStatus.getSignal().setTimestamp(time);
	
	tmp_prev_status = status.getSignal().getValue();
}

std::vector<int> ReadSensorData::defineCells(eeros::math::Matrix<1,15,int> data) {
	std::vector<int> out;
	int i = 0;
	
	for(i = 0; i < 15; i++){
		if (data[i] == -1)
			return out;
		else 
			out[i] = data[i];
	}
	return out;
}

bool ReadSensorData::isSomebody(std::vector<int> data) {
	for (int i = 0; i < data.size(); i++){
		if (data[i] == true)
			return true;
	}
}
