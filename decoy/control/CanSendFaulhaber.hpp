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
			virtual Output<bool>& getPdoSignalOut(){
				return pdoRequested;
			}
			  
		private:
			int sock;
			int err;
			std::vector<int> lastPosValue;
			
		protected:
			std::vector<eeros::control::Input<int>*> in;
			std::vector<int> nodes;
			eeros::control::Output<bool> pdoRequested;
		};
	};
};

#endif // CH_NTB_CANSENDFAULHABER_HPP_