#ifndef CH_NTB_PATHOS_DECOY_HOMING_HPP_
#define CH_NTB_PATHOS_DECOY_HOMING_HPP_

#include "../control/ControlSystem_decoy.hpp"
#include "../safety/SafetyProperties_decoy.hpp"
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>

namespace pathos{
	namespace decoy{
		
		class Homing : public eeros::sequencer::Sequence<> {

		public:
			Homing(eeros::sequencer::Sequencer* sequencer, pathos::decoy::ControlSystem_decoy* controlSys, eeros::safety::SafetySystem* safetySys);

			virtual void init();
			virtual bool checkPreCondition();
			virtual void run();
			virtual bool checkPostCondition();
			virtual void exit();
			
		private:
			bool isTerminating();
			
			pathos::decoy::ControlSystem_decoy* controlSys;
			eeros::safety::SafetySystem* safetySys;
		};
	};
};

#endif // CH_NTB_PATHOS_DECOY_HOMING_HPP_ 