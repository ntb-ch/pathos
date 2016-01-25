#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <eeros/core/SimpleConfig.hpp>
 
using namespace eeros;
 
class PeepingPanelConfig : public SimpleConfig {
	public:
		PeepingPanelConfig(const char *name) : SimpleConfig(name) {
			add("peepSpeed"    , peep_speed);
			add("peepDirection", peep_direction);
			add("peepPosition" , peep_position);
		}

		double peep_speed;
		double peep_direction; 
		double peep_position; 
};
 
 
// int main(int argc, char **argv) {
//     std::cout << "Config demo started!" << std::endl;
//                 std::string fileName("config.txt");          // choose an appropriate path
//                 PeepingPanelsConfig configFile(fileName.c_str());
//                 configFile.load();
//                 std::cout << "Config read from file: value1 = " << configFile.value1 << std::endl;
//                 std::cout << "Config read from file: value2 = " << configFile.value2 << std::endl;
//                 configFile.value1 += 100;
//                 configFile.value2 += 3.5;
//                 configFile.save();
//  
//     return 0;
// }