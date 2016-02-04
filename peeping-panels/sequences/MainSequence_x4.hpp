#ifndef CH_NTB_PEEPINGPANEL_MAINSEQUENCE_X4_HPP_
#define CH_NTB_PEEPINGPANEL_MAINSEQUENCE_X4_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/PeepingPanelControlSystem.hpp"

#include "Peep_out.hpp"
#include "Peep_in.hpp"
#include "Teaching.hpp"

namespace pathos {
	namespace peepingpanel {
		
		class MainSequence_x4 : public eeros::sequencer::Sequence<void> {

		public:
			MainSequence_x4(eeros::sequencer::Sequencer* sequencer, PeepingPanelControlSystem* controlSys, eeros::safety::SafetySystem* safetySys);
			
			virtual bool checkPreCondition();
			virtual void run();
			virtual void exit();
			
		private:
			Peep_out peep_out_s;
			Peep_in  peep_in_s;
			Teaching teaching_s;
			
			bool isTerminating();
			
			peepingpanel::PeepingPanelControlSystem* controlSys;
			eeros::safety::SafetySystem* safetySys;
			
			double peep_time, peep_position, peep_direction;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_MAINSEQUENCE_X4_HPP_  
