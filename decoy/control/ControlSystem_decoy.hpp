#ifndef CH_NTB_DECOY_CONTROLSYSTEM_HPP_
#define CH_NTB_DECOY_CONTROLSYSTEM_HPP_

#include <eeros/core/Runnable.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/Constant.hpp>
#include "CanSendFaulhaber.hpp"
#include "CanReceiveFaulhaber.hpp"
#include "RadToPulses.hpp"
#include <canopen-faulhaber-drv.h>

namespace pathos {
	namespace decoy {

		class ControlSystem_decoy {
		
		public:
			ControlSystem_decoy(int canSock, double ts);
			virtual ~ControlSystem_decoy();
			
			virtual double getActualPos_rad(int node);
			virtual int getActualPos_pulses(int node);
			
			virtual bool isHomed(int node);
			virtual bool isHomingError(int node);
			virtual bool setPointReceived(int node);
			virtual bool setPointReached(int node);
			virtual uint16_t getStatusWord(int node);
			
			eeros::control::CanSendFaulhaber canSend;
			eeros::control::CanReceiveFaulhaber canReceive;
			
			eeros::control::Constant<int> setPosPulses_node1;
			eeros::control::Constant<int> setPosPulses_node2;
			eeros::control::Constant<int> setPosPulses_node3;
			eeros::control::Constant<int> setPosPulses_node4;
			eeros::control::Constant<int> setPosPulses_node5;
			
			eeros::control::TimeDomain timedomain;
					
		private:
			int socket;
	
			
		};
	};
};

#endif // CH_NTB_DECOY_CONTROLSYSTEM_HPP_