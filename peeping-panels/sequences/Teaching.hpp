#ifndef CH_NTB_PEEPINGPANEL_TEACHING_HPP_
#define CH_NTB_PEEPINGPANEL_TEACHING_HPP_

#include "../control/ControlSystem_Teach.hpp"
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../types.hpp"

namespace pathos{
	namespace peepingpanel {
		class Teaching : public eeros::sequencer::Sequence<> {

		public:
			Teaching(eeros::sequencer::Sequencer* sequencer, pathos::peepingpanel::ControlSystem_Teach* controlSys, eeros::safety::SafetySystem* safetySys);

			virtual void init();
			virtual bool checkPreCondition();
			virtual void run();
			virtual bool checkPostCondition();
			virtual void exit();
			
		private:
			bool isTerminating();
			
			pathos::peepingpanel::ControlSystem_Teach* controlSys;
			eeros::safety::SafetySystem* safetySys;
		};
	}
}

#endif // CH_NTB_PEEPINGPANEL_TEACHING_HPP_ 