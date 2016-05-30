#ifndef CH_NTB_CANRECEIVEFAULHABER_HPP_
#define CH_NTB_CANRECEIVEFAULHABER_HPP_

#include <eeros/control/Block.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/control/Input.hpp>
#include <canopen.h>
#include <canopen-drv.h>
#include <map>

namespace eeros {
	namespace control {

		class CanReceiveFaulhaber: public Block {
		  
		public:
			CanReceiveFaulhaber(int sock, std::initializer_list<uint32_t> node, std::initializer_list<uint8_t> functionCode);
			virtual ~CanReceiveFaulhaber();
			
			virtual void run();
			virtual eeros::control::Input<bool>& getPdoSignalIn() {
				return pdoRequested;
			}
						
			struct PdoUserData{
				uint32_t nodeId;
				int16_t drvCtrl;
				int32_t userData;
			};
						
			int getPdoValue(int32_t node, uint8_t functionCode, int16_t* drvCtrl, int32_t* userData) ;

		private:
			int sock;
			canopen_frame_t readFrame;
			int readLen;
		protected:
			std::vector<eeros::control::Output<int>*> out;
			std::vector<uint32_t> nodes;
			std::vector<uint8_t> functionCodes;
			std::map<uint8_t, PdoUserData> pdoData;
			eeros::control::Input<bool> pdoRequested;
		};
	};
};

#endif // CH_NTB_CANRECEIVEFAULHABER_HPP_