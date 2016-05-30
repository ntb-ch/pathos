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
			
			virtual bool isOperationEnabled();
			virtual int16_t getDrivesStatus();
			
			eeros::control::CanSendFaulhaber canSend;
			eeros::control::CanReceiveFaulhaber canReceive;
			eeros::control::Constant<double> setPosRad_armRight;
			eeros::control::Constant<double> setPosRad_armLeft;
			pathos::decoy::RadToPulses radToPulses_ar;
			pathos::decoy::RadToPulses radToPulses_al;
			
			eeros::control::TimeDomain timedomain;
					
		private:
			int socket;
	
			
		};
	};
};

#endif // CH_NTB_DECOY_CONTROLSYSTEM_HPP_