#ifndef CH_NTB_PEEPINGPANEL_READSENSORDATA_HPP_
#define CH_NTB_PEEPINGPANEL_READSENSORDATA_HPP_

#include <eeros/control/Block.hpp>
#include <eeros/control/Output.hpp>
#include <atomic>
#include "../types.hpp"
#include "../constants.hpp"
#include "../sensors/SensorsThread.hpp"
#include <array>

namespace pathos {
	namespace peepingpanel {
	
		class ReadSensorData: public eeros::control::Block {
			
		public:
			static constexpr int NUM_ID = 48;
// 			using Array = std::array<bool,NUM_ID>;
			
			enum {
				visible  = 0,
				close    = 1,
				near     = 2,
				far      = 3,
				somebody = 4,
				nobody   = 5,
			};
			
			ReadSensorData(SensorsThread* sensThread, std::array<int,60> sensAreas);
			
			virtual eeros::control::Output<int>& getOutStatus();
			virtual eeros::control::Output<int>& getOutPrevStatus();
			virtual void run();
			
		protected:
			eeros::control::Output<int> status;
			eeros::control::Output<int> prevStatus;
			
			virtual std::vector<int> defineCells(std::array<int,15> data);
			virtual bool isSomebody (std::array<bool,NUM_ID> sensors_data, std::vector<int> data);
			
			SensorsThread* sensorsThread;
			std::array<int,60> sensorsAreas;
			std::vector<int> id_visible, id_close, id_near, id_far, id_all;
		};
	};
}

#endif /* CH_NTB_PEEPINGPANEL_READSENSORDATA_HPP_ */
