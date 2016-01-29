#ifndef CH_NTB_PEEPINGPANEL_MAINSEQUENCE_TEACH_HPP_
#define CH_NTB_PEEPINGPANEL_MAINSEQUENCE_TEACH_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/PeepingPanelControlSystem.hpp"
#include "Teaching.hpp"

namespace pathos {
	namespace peepingpanel {
		
		class MainSequence_teach : public eeros::sequencer::Sequence<void> {

		public:
			MainSequence_teach(eeros::sequencer::Sequencer* sequencer, PeepingPanelControlSystem* controlSys, eeros::safety::SafetySystem* safetySys);
			
			virtual bool checkPreCondition();
			virtual void run();
			virtual void exit();
			
		private:
			Teaching teaching_s;
			
			bool isTerminating();
			
			peepingpanel::PeepingPanelControlSystem* controlSys;
			eeros::safety::SafetySystem* safetySys;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_MAINSEQUENCE_TEACH_HPP_  
