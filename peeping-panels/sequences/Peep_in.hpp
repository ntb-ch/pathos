#ifndef CH_NTB_PEEPINGPANEL_PEEPIN_HPP_
#define CH_NTB_PEEPINGPANEL_PEEPIN_HPP_

#include "../control/PeepingPanelControlSystem.hpp"
#include "../safety/PeepingPanelSafetyProperties_x4.hpp"
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

namespace pathos {
	namespace peepingpanel{
		
		class Peep_in : public eeros::sequencer::Sequence<> {

		public:
			Peep_in(eeros::sequencer::Sequencer* sequencer, pathos::peepingpanel::PeepingPanelControlSystem* controlSys, eeros::safety::SafetySystem* safetySys);

			virtual void init();
			virtual bool checkPreCondition();
			virtual void run();
			virtual bool checkPostCondition();
			virtual void exit();
			
			virtual void setPeepAngle(double angle);
			virtual void setPeepDirection(double direction);
			
		private:
			double peepAngle = 0.0; 
			double peepDirection = 1.0;
			
			bool isTerminating();
			bool isStopping();
						
			peepingpanel::PeepingPanelControlSystem* controlSys;
			eeros::safety::SafetySystem* safetySys;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_PEEPIN_HPP_ 