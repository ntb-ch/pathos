#ifndef CH_NTB_DECOY_CONTROLSYSTEM_HPP_
#define CH_NTB_DECOY_CONTROLSYSTEM_HPP_

#include <eeros/core/Runnable.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/Constant.hpp>
#include "CanSendFaulhaber.hpp"
#include "CanReceiveFaulhaber.hpp"
#include <canopen-faulhaber-drv.h>

namespace pathos {
	namespace decoy {

		class ControlSystem_decoy {
		
		public:
			ControlSystem_decoy(int canSock, double ts);
			virtual ~ControlSystem_decoy();
		
			eeros::control::CanSendFaulhaber canSend;
			eeros::control::CanReceiveFaulhaber canReceive;
			eeros::control::Constant<int> setPos;
			eeros::control::TimeDomain timedomain;
		
		private:
			// ...
		};
	};
};

#endif // CH_NTB_DECOY_CONTROLSYSTEM_HPP_