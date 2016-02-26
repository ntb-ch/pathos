#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <eeros/core/SimpleConfig.hpp>
 
using namespace eeros;
 
class PeepingPanelConfig : public SimpleConfig {
	public:
		PeepingPanelConfig(const char *name) : SimpleConfig(name) {
			add("motorsConnected" , motors_config   );
			
			add("peepTime1"       , peep_time1      );
			add("peepDirection1"  , peep_direction1 );
			add("peepPosition1"   , peep_position1  );
			                                        
			add("peepTime2"       , peep_time2      );
			add("peepDirection2"  , peep_direction2 );
			add("peepPosition2"   , peep_position2  );
			                                        
			add("peepTime3"       , peep_time3      );
			add("peepDirection3"  , peep_direction3 );
			add("peepPosition3"   , peep_position3  );
			                                        
			add("peepTime4"       , peep_time4      );
			add("peepDirection4"  , peep_direction4 );
			add("peepPosition4"   , peep_position4  );
			
			add("visible1" , visible1 );
			add("close1"   , close1   );
			add("near1"    , near1    );
			add("far1"     , far1     );
			                          
			add("visible2" , visible2 );
			add("close2"   , close2   );
			add("near2"    , near2    );
			add("far2"     , far2     );
			                          
			add("visible3" , visible3 );
			add("close3"   , close3   );
			add("near3"    , near3    );
			add("far3"     , far3     );
			                          
			add("visible4" , visible4 );
			add("close4"   , close4   );
			add("near4"    , near4    );
			add("far4"     , far4     );
			
		}

		double motors_config;
		
		double peep_time1, peep_time2, peep_time3, peep_time4;
		double peep_direction1, peep_direction2, peep_direction3, peep_direction4; 
		double peep_position1, peep_position2, peep_position3, peep_position4; 
		
		std::array<int,15> visible1, close1, near1, far1;
		std::array<int,15> visible2, close2, near2, far2;
		std::array<int,15> visible3, close3, near3, far3;
		std::array<int,15> visible4, close4, near4, far4;
};
