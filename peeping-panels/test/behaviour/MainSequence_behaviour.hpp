#ifndef CH_NTB_PEEPINGPANEL_MAINSEQUENCE_BEHAVIOUR_HPP_
#define CH_NTB_PEEPINGPANEL_MAINSEQUENCE_BEHAVIOUR_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../../control/PeepingPanelControlSystem.hpp"

#include "../../sequences/Peep_out.hpp"
#include "../../sequences/Peep_in.hpp"
#include "../../sequences/Teaching.hpp"

namespace pathos {
	namespace peepingpanel {
		
		class MainSequence_behaviour : public eeros::sequencer::Sequence<void> {

		public:
			MainSequence_behaviour(eeros::sequencer::Sequencer* sequencer, PeepingPanelControlSystem* controlSys, eeros::safety::SafetySystem* safetySys, std::array<double,4> configData);
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
			
			double peep_time, peep_angle;
			char peep_direction;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_MAINSEQUENCE_BEHAVIOUR_HPP_  
