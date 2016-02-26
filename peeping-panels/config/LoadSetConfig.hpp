#include <iostream>
#include "PeepingPanelConfig.hpp"

using Array3  = std::array<double, 3 >;
using Array13 = std::array<double, 13>;
using Array15 = std::array<int, 15>;    // cells IDs
using Array60 = std::array<int, 60>;    // cells IDs

// *** 1. Motor config, peep time, angle, direction

Array13 configPanelData(PeepingPanelConfig configFile) {
	Array13 configData;
	
	// *** Config motors *** //
	if (configFile.motors_config >= 0 && configFile.motors_config < 16) 
		configData[0] = configFile.motors_config;
	else
		throw EEROSException("Wrong motors config nr (0 to 15)");
	
	// *** Panel 1 *** //
	if (configFile.peep_time1 >= 0) 
		configData[1] = configFile.peep_time1;
	else
		throw EEROSException("Wrong config time ( > 0)");
	
	if(configFile.peep_direction1 == 1.0 || configFile.peep_direction1 == -1.0)
		configData[2] = configFile.peep_direction1;
	else
		throw EEROSException("Wrong direction input, check input file");
	
	configData[3] = configFile.peep_position1;
	
	// *** Panel 2 *** //
	if (configFile.peep_time2 >= 0) 
		configData[4] = configFile.peep_time2;
	else
		throw EEROSException("Wrong config time ( > 0)");
	
	if(configFile.peep_direction2 == 1.0 || configFile.peep_direction2 == -1.0)
		configData[5] = configFile.peep_direction2;
	else
		throw EEROSException("Wrong direction input, check input file");
	
	configData[6] = configFile.peep_position2;
		
	// *** Panel 3 *** //
	if (configFile.peep_time3 >= 0) 
		configData[7] = configFile.peep_time3;
	else
		throw EEROSException("Wrong config time ( > 0)");
	
	if(configFile.peep_direction3 == 3.0 || configFile.peep_direction3 == -1.0)
		configData[8] = configFile.peep_direction3;
	else
		throw EEROSException("Wrong direction input, check input file");
	
	configData[9] = configFile.peep_position3;
	
	// *** Panel 4 *** //
	if (configFile.peep_time4 >= 0) 
		configData[10] = configFile.peep_time4;
	else
		throw EEROSException("Wrong config time ( > 0)");
	
	if(configFile.peep_direction4 == 1.0 || configFile.peep_direction4 == -1.0)
		configData[11] = configFile.peep_direction4;
	else
		throw EEROSException("Wrong direction input, check input file");
	
	configData[12] = configFile.peep_position4;
	
	return configData;
}

std::vector<Array3> setSystemData(Array13 configData) { 
	std::vector<Array3>configSystem;
	
	if(configData[0] == 1){              // 0 0 0 1
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config4);
	}
	else if (configData[0] == 2) {       // 0 0 1 0
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		configSystem.push_back(config3);
	}
	else if (configData[0] == 3) {       // 0 0 1 1
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config3);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 4) {       // 0 1 0 0
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		configSystem.push_back(config2);
	}
	else if (configData[0] == 5) {       // 0 1 0 1
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config2);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 6) {       // 0 1 1 0
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		configSystem.push_back(config2);
		configSystem.push_back(config3);
	}
	else if (configData[0] == 7) {       // 0 1 1 1
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config2);
		configSystem.push_back(config3);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 8) {       // 1 0 0 0
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		configSystem.push_back(config1);
	}
	else if (configData[0] == 9) {       // 1 0 0 1
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config1);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 10) {      // 1 0 1 0
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		configSystem.push_back(config1);
		configSystem.push_back(config3);
	}
	else if (configData[0] == 11) {      // 1 0 1 1
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config1);
		configSystem.push_back(config3);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 12) {      // 1 1 0 0
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		configSystem.push_back(config1);
		configSystem.push_back(config2);
	}
	else if (configData[0] == 13) {      // 1 1 0 1
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config1);
		configSystem.push_back(config2);
		configSystem.push_back(config4);
	}
	else if (configData[0] == 14) {      // 1 1 1 0
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		configSystem.push_back(config1);
		configSystem.push_back(config2);
		configSystem.push_back(config3);
	}
	else if (configData[0] == 15) {      // 1 1 1 1 
		Array3 config1 = {configData[1] , configData[2] , configData[3] };
		Array3 config2 = {configData[4] , configData[5] , configData[6] };
		Array3 config3 = {configData[7] , configData[8] , configData[9] };
		Array3 config4 = {configData[10], configData[11], configData[12]};
		configSystem.push_back(config1);
		configSystem.push_back(config2);
		configSystem.push_back(config3);
		configSystem.push_back(config4);
	}
	else {
		throw eeros::EEROSException("Invalid config number");
	}
	return configSystem;
}

// *** 3. Load system configuration (all)
void LoadConfig_system(std::string fileName, std::array<double,13>* allPanelsData, std::vector<Array3>* systemDataPtr, 
	Array60* sensArea_1, Array60*  sensArea_2, Array60* sensArea_3, Array60* sensArea_4) {/*eeros::math::Matrix<16,15,int>* allSensData) {*/
	PeepingPanelConfig configFile(fileName.c_str());
	configFile.load();
	
	// 1. Config data
	Array13 configData = configPanelData(configFile);
	*systemDataPtr = setSystemData(configData);
	*allPanelsData = configData;
	
	// 2. Sensors data
	
	// TODO substitute with read from config file
	Array15 visible1 = {1, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 close1   = {5, 6,  7, -1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 near1    = {8, 9, -1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 far1     = {10, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
	Array15 visible2 = {1, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 close2   = {5, 6,  7, -1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 near2    = {8, 9, -1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 far2     = {10, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
	Array15 visible3 = {1, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 close3   = {5, 6,  7, -1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 near3    = {8, 9, -1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 far3     = {10, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
	Array15 visible4 = {1, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 close4   = {5, 6,  7, -1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 near4    = {8, 9, -1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  }; 
	Array15 far4     = {10, 2,  3,  4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
	// TODO end
		
	Array60 sa1, sa2, sa3, sa4;
	
	int i = 0;
	while(i<60){
		if(i<15){
			sa1[i] = visible1[i]; 
			sa2[i] = visible2[i];
			sa3[i] = visible3[i]; 
			sa4[i] = visible4[i];
		}
		else if (i < 30){
			sa1[i] = close1[i-15]; 
			sa2[i] = close2[i-15];
			sa3[i] = close3[i-15]; 
			sa4[i] = close4[i-15];
		}
		else if (i < 45){
			sa1[i] = near1[i-30];
			sa2[i] = near2[i-30];
			sa3[i] = near3[i-30];
			sa4[i] = near4[i-30];
		}
		else if (i < 60){
			sa1[i] = far1[i-45];
			sa2[i] = far2[i-45];
			sa3[i] = far3[i-45];
			sa4[i] = far4[i-45];
		}
		i++;
	}
	
	*sensArea_1 = sa1;
	*sensArea_2 = sa2;
	*sensArea_3 = sa3;
	*sensArea_4 = sa4;
	
	std::cout << "Configuration file loaded"  << std::endl;
}

