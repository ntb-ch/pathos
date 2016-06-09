#ifndef CH_NTB_DECOY_MAINSEQUENCE_HPP_
#define CH_NTB_DECOY_MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/ControlSystem_decoy.hpp"
#include "ArmMotion.hpp"

namespace pathos {
	namespace decoy {
		
		class MainSequence_decoy : public eeros::sequencer::Sequence<void> {

		public:
			MainSequence_decoy(eeros::sequencer::Sequencer* sequencer, pathos::decoy::ControlSystem_decoy* controlSys, eeros::safety::SafetySystem* safetySys);
			
			virtual bool checkPreCondition();
			virtual void run();
			virtual void exit();
			
		private:
			ArmMotion armMotion_s;
			
			bool isTerminating();
			
			pathos::decoy::ControlSystem_decoy* controlSys;
			eeros::safety::SafetySystem* safetySys;
			
		};
	};
};

#endif // CH_NTB_DECOY_MAINSEQUENCE_HPP_  
