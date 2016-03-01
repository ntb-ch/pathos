#include "../control/ControlSystem_Peep.hpp"
#include "../control/ControlSystem_Teach.hpp"
#include <iostream>
#include "../types.hpp"

using namespace pathos::peepingpanel;

std::vector<ControlSystem_Peep*> CreateControlSystem(AllConfigArray configData, 
													 SensorsAreasArray sensorsArea_1, SensorsAreasArray sensorsArea_2, 
													 SensorsAreasArray sensorsArea_3, SensorsAreasArray sensorsArea_4, 
													 SensorsThread* sensorsThread) 

{
	std::vector<ControlSystem_Peep*> controlSystems;
	int config = configData[0];
		
	if(config == 0)
		throw eeros::EEROSException("No motor connected");
	else if(config == 1 ){        // 0 0 0 1
		std::cout << "motor 4 connected" << std::endl;
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4));
	}                                                                                 
	else if(config == 2 ){        // 0 0 1 0            
		std::cout << "motor 3 connected" << std::endl;
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
	}                                                                 
	else if(config == 3 ){       // 0 0 1 1                           
		std::cout << "motor 3 and 4 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4));
	}                                                                 
	else if(config == 4 ){       // 0 1 0 0                           
		std::cout << "motor 2 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
	}                                                                 
	else if(config == 5 ){       // 0 1 0 1                           
		std::cout << "motor 2 and 4 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4));
	}                                                                 
	else if(config == 6 ){       // 0 1 1 0                           
		std::cout << "motor 2 and 3 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3)); 
	}                                                                 
	else if(config == 7 ){      // 0 1 1 1                            
		std::cout << "motor 2, 3 and 4 connected" << std::endl;  
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4)); 
	}                                                                 
	else if(config == 8 ){     // 1 0 0 0               
		std::cout << "motor 1 connected" << std::endl;    
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));  
	}                                                                 
	else if(config == 9 ){     // 1 0 0 1                             
		std::cout << "motor 1 and 4 connected" << std::endl;  
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4));
	}                                                                 
	else if(config == 10){     // 1 0 1 0                             
		std::cout << "motor 1 and 3 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3)); 
	}                                                                 
	else if(config == 11){      // 1 0 1 1                            
		std::cout << "motor 1, 3 and 4 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4)); 
	}                                                                 
	else if(config == 12){     // 1 1 0 0                             
		std::cout << "motor 1 and 2 connected" << std::endl;      
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2)); 
	}                                                                 
	else if(config == 13){     // 1 1 0 1                             
		std::cout << "motor 1, 2 and 4 connected" << std::endl; 
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4)); 
	}                                                                 
	else if(config == 14){    // 1 1 1 0                              
		std::cout << "motor 1, 2 and 3 connected" << std::endl;  
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
	}                                                                 
	else if(config == 15){    // 1 1 1 1                              
		std::cout << "all motors connected" << std::endl;  
		controlSystems.push_back(new ControlSystem_Peep("enc1", "dac1", sensorsThread, sensorsArea_1));
		controlSystems.push_back(new ControlSystem_Peep("enc2", "dac2", sensorsThread, sensorsArea_2));
		controlSystems.push_back(new ControlSystem_Peep("enc3", "dac3", sensorsThread, sensorsArea_3));
		controlSystems.push_back(new ControlSystem_Peep("enc4", "dac4", sensorsThread, sensorsArea_4));
	}
	else
		throw eeros::EEROSException("Invalid motor config value");
	
	return controlSystems;
}

std::vector<ControlSystem_Teach*> CreateControlSystem_teach(AllConfigArray configData) {
	std::vector<ControlSystem_Teach*> controlSystems;
	int config = configData[0];
	
	if(config == 0)
		throw eeros::EEROSException("No motor connected");
	else if(config == 1 ){        // 0 0 0 1
		std::cout << "motor 4 connected" << std::endl;
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4"));
	}                                                                 
	else if(config == 2 ){        // 0 0 1 0                          
		std::cout << "motor 3 connected" << std::endl;                
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
	}                                                                 
	else if(config == 3 ){       // 0 0 1 1                           
		std::cout << "motor 3 and 4 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4"));
	}                                                                 
	else if(config == 4 ){       // 0 1 0 0                           
		std::cout << "motor 2 connected" << std::endl;                
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
	}                                                                 
	else if(config == 5 ){       // 0 1 0 1                           
		std::cout << "motor 2 and 4 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4"));
	}                                                                 
	else if(config == 6 ){       // 0 1 1 0                           
		std::cout << "motor 2 and 3 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3")); 
	}                                                                 
	else if(config == 7 ){      // 0 1 1 1                            
		std::cout << "motor 2, 3 and 4 connected" << std::endl;       
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4")); 
	}                                                                 
	else if(config == 8 ){     // 1 0 0 0                             
		std::cout << "motor 1 connected" << std::endl;                
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1")); 
	}                                                                 
	else if(config == 9 ){     // 1 0 0 1                             
		std::cout << "motor 1 and 4 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4"));
	}                                                                 
	else if(config == 10){     // 1 0 1 0                             
		std::cout << "motor 1 and 3 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3")); 
	}                                                                 
	else if(config == 11){      // 1 0 1 1                            
		std::cout << "motor 1, 3 and 4 connected" << std::endl;       
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4")); 
	}                                                                 
	else if(config == 12){     // 1 1 0 0                             
		std::cout << "motor 1 and 2 connected" << std::endl;          
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2")); 
	}                                                                 
	else if(config == 13){     // 1 1 0 1                             
		std::cout << "motor 1, 2 and 4 connected" << std::endl;       
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4")); 
	}                                                                 
	else if(config == 14){    // 1 1 1 0                              
		std::cout << "motor 1, 2 and 3 connected" << std::endl;       
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
	}                                                                 
	else if(config == 15){    // 1 1 1 1                              
		std::cout << "all motors connected" << std::endl;             
		controlSystems.push_back(new ControlSystem_Teach("enc1", "dac1"));
		controlSystems.push_back(new ControlSystem_Teach("enc2", "dac2"));
		controlSystems.push_back(new ControlSystem_Teach("enc3", "dac3"));
		controlSystems.push_back(new ControlSystem_Teach("enc4", "dac4"));
	}
	else
		throw eeros::EEROSException("Invalid motor config value");
	
	return controlSystems;
}

