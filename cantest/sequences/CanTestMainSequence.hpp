#ifndef CH_NTB_CANTESTMAINSEQUENCE_HPP_
#define CH_NTB_CANTESTMAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "../control/CanControlSystem.hpp"

class CanTestMainSequence : public eeros::sequencer::Sequence<void> {

public:
	CanTestMainSequence(eeros::sequencer::Sequencer* sequencer, CanControlSystem* controlSys, eeros::safety::SafetySystem* safetySys);
	
	virtual bool checkPreCondition();
	virtual void run();
	virtual void exit();
	
private:
	
	bool isTerminating();
	
	CanControlSystem* controlSys;
	eeros::safety::SafetySystem* safetySys;
};
		
#endif // CH_NTB_CANTESTMAINSEQUENCE_HPP_  
