#ifndef CH_NTB_CANCONTROLSYSTEM_HPP_
#define CH_NTB_CANCONTROLSYSTEM_HPP_

#include <eeros/core/Runnable.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/Constant.hpp>
#include "CanSendFaulhaber.hpp"
#include "CanReceiveFaulhaber.hpp"
#include <canopen-faulhaber-drv.h>

class CanControlSystem {
  
public:
	CanControlSystem(int canSock, double ts);
	virtual ~CanControlSystem();
  
	eeros::control::CanSendFaulhaber canSend;
	eeros::control::CanReceiveFaulhaber canReceive;
	eeros::control::Constant<int> setPos;
	eeros::control::TimeDomain timedomain;
  
private:
	// ...
};

#endif // CH_NTB_CANCONTROLSYSTEM_HPP_