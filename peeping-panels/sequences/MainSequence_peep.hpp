#ifndef CH_NTB_PEEPINGPANEL_MAINSEQUENCE_PEEP_HPP_
#define CH_NTB_PEEPINGPANEL_MAINSEQUENCE_PEEP_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/ControlSystem_Peep.hpp"

#include "Peep_out.hpp"
#include "Peep_in.hpp"
#include "../types.hpp"

namespace pathos {
	namespace peepingpanel {
		
		class MainSequence_peep : public eeros::sequencer::Sequence<void> {

		public:
			MainSequence_peep(eeros::sequencer::Sequencer* sequencer, ControlSystem_Peep* controlSys, eeros::safety::SafetySystem* safetySys, PanelConfigArray configData);
			
			virtual bool checkPreCondition();
			virtual void run();
			virtual void exit();
			
		private:
			Peep_out peep_out_s;
			Peep_in  peep_in_s;
			
			bool isTerminating();
			
			peepingpanel::ControlSystem_Peep* controlSys;
			eeros::safety::SafetySystem* safetySys;
			
			// configData = time, direction, position
			double peep_time, peep_angle;
			char peep_direction;
			int panelSize; 
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_MAINSEQUENCE_PEEP_HPP_  
