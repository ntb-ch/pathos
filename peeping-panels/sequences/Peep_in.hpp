#ifndef CH_NTB_PEEPINGPANEL_PEEPIN_HPP_
#define CH_NTB_PEEPINGPANEL_PEEPIN_HPP_

#include "../control/ControlSystem_Peep.hpp"
#include "../safety/SafetyProperties_Peep.hpp"
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

namespace pathos {
	namespace peepingpanel{
		
		class Peep_in : public eeros::sequencer::Sequence<> {

		public:
			Peep_in(eeros::sequencer::Sequencer* sequencer, pathos::peepingpanel::ControlSystem_Peep* controlSys, eeros::safety::SafetySystem* safetySys);

			virtual void init();
			virtual bool checkPreCondition();
			virtual void run();
			virtual bool checkPostCondition();
			virtual void exit();
			
			virtual void setMotionCurve(std::string fn, double t);
			
		private:
			double time = 1.0;
			std::string filename;
			
			bool isTerminating();
			bool isStopping();
						
			peepingpanel::ControlSystem_Peep* controlSys;
			eeros::safety::SafetySystem* safetySys;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_PEEPIN_HPP_ 