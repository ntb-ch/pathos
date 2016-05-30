#ifndef CH_NTB_CANPATHMAINSEQUENCE_HPP_
#define CH_NTB_CANPATHMAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/CanControlSystem.hpp"

class CanPathMainSequence : public eeros::sequencer::Sequence<void> {

public:
	CanPathMainSequence(eeros::sequencer::Sequencer* sequencer, CanControlSystem* controlSys, eeros::safety::SafetySystem* safetySys);
	
	virtual bool checkPreCondition();
	virtual void run();
	virtual void exit();
	
private:
	
	bool isTerminating();
	
	CanControlSystem* controlSys;
	eeros::safety::SafetySystem* safetySys;
};
		
#endif // CH_NTB_CANPATHMAINSEQUENCE_HPP_  
