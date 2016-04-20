#ifndef ORG_EEROS_CONTROL_CANHANDLE_HPP_
#define ORG_EEROS_CONTROL_CANHANDLE_HPP_

#include <canopen-drv.h>
#include <canopen-faulhaber-drv.h>

#include <eeros/core/EEROSException.hpp>

namespace eeros {
	namespace control {

		class CanHandle {
		  
		public:
			CanHandle(std::string can_iface);
			virtual ~CanHandle();
			int getSocket();
			  
		private:
			int sock;
			struct sockaddr_can addr;
			struct ifreq ifr;
		  
		};
	};
};

#endif // ORG_EEROS_CONTROL_CANHANDLE_HPP_