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
			using Array = std::array<bool,NUM_ID>;
			
			enum {
				visible  = 0,
				close    = 1,
				near     = 2,
				far      = 3,
				somebody = 4,
				nobody   = 5,
			};
			
			ReadSensorData(SensorsThread* sensorsdata, eeros::math::Matrix<4,15,int> sensAreas);
			
			virtual eeros::control::Output<int>& getOutStatus();
			virtual eeros::control::Output<int>& getOutPrevStatus();
			virtual void run();
		
		protected:
			eeros::control::Output<int> status;
			eeros::control::Output<int> prevStatus;
			
			virtual std::vector<int> defineCells(eeros::math::Matrix<1,15,int> data);
			virtual bool             isSomebody (std::vector<int>   data);
			
			SensorsThread* sensorsData;
			eeros::math::Matrix<4,15,int> sensorsAreas;
			std::vector<int> id_visible, id_close, id_near, id_far, id_all;
		};
	};
}

#endif /* CH_NTB_PEEPINGPANEL_READSENSORDATA_HPP_ */
