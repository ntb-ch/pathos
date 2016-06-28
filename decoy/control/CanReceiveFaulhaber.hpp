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
			
			virtual Output<uint8_t>& getFcOut(uint32_t node);
			virtual Output<uint16_t>& getdrvCtrlOut(uint32_t node);
			virtual Output<int32_t>& getUserDataOut(uint32_t node);
			virtual Output<bool> getOpEnabledOut();
			
			virtual void enable();
			virtual void disable();
						
			struct PdoUserData{
				uint8_t functionCode;
				uint16_t drvCtrl;
				int32_t userData;
			};
						
// 			int getPdoValue(uint32_t node, uint8_t functionCode, uint16_t* drvCtrl, uint32_t* userData) ;

		private:
			int sock;
			canopen_frame_t readFrame;
			int readLen;
			bool enabled = false;
			int debCnt = 0;
			std::vector<bool> axisEnabled;
			bool allAxisEnabled = false;
			std::map<uint32_t, PdoUserData>::iterator mIt;
			bool first = true;
			
		protected:
			std::vector<eeros::control::Output<uint8_t>*> fcOut;
			std::vector<eeros::control::Output<uint16_t>*> drvCtrlOut;
			std::vector<eeros::control::Output<int32_t>*> userDataOut;
			eeros::control::Output<bool> isEnabled;
			std::vector<uint32_t> nodes;
			std::vector<uint8_t> functionCodes;
// 			std::map<uint8_t, PdoUserData> pdoData;
			std::vector<std::map<uint32_t, PdoUserData>> nodePdoData;
			eeros::control::Input<bool> pdoRequested;
		};
	};
};

#endif // CH_NTB_CANRECEIVEFAULHABER_HPP_