#ifndef CH_NTB_CANSENDFAULHABER_HPP_
#define CH_NTB_CANSENDFAULHABER_HPP_

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/core/EEROSException.hpp>
#include <canopen-drv.h>
#include <canopen.h>

namespace eeros {
	namespace control {
		class CanSendFaulhaber : public Block {
		public:
			CanSendFaulhaber(int sock, std::initializer_list<int> node);
			virtual ~CanSendFaulhaber();
			
			virtual void run();
			eeros::control::Input<int>* getInput(int node);
			virtual void initiatePdoRequest(int node, uint8_t function_code);
			virtual void sendPdo(int node, uint8_t function_code, uint32_t userData);
			virtual Output<bool>& getPdoSignalOut(){
				return pdoRequested;
			}
			
			virtual void enable();
			virtual void disable();
			  
		private:
			int sock;
			int err;
			std::vector<int> lastPosValue;
			bool enabled = false;
			int stateCnt = 0;
			
		protected:
			std::vector<eeros::control::Input<int>*> in;
			std::vector<int> nodes;
			eeros::control::Output<bool> pdoRequested;
		};
	};
};

#endif // CH_NTB_CANSENDFAULHABER_HPP_