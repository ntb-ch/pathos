#ifndef CH_NTB_PEEPINGPANEL_SENSORTHREAD_HPP_
#define CH_NTB_PEEPINGPANEL_SENSORTHREAD_HPP_

#include <eeros/core/Thread.hpp>
#include <atomic>

namespace pathos {
	namespace peepingpanel {
	
		class SensorsThread : public eeros::Thread {
		
		public:
			static constexpr int NUM_ID = 48;
			using Array = std::array<bool,NUM_ID>;
			
			SensorsThread();
			virtual ~SensorsThread();
			
			virtual void stop();
			virtual bool isRunning();
			
			const Array& getBuffer();
			
		private:
			virtual void run();
			Array& getWriteBuffer();
			void flip();
			
			bool running;
			Array detection1;
			Array detection2;
			Array detection3;
			
			std::atomic<Array*> read_ptr;
		};
	};
};

#endif // CH_NTB_PEEPINGPANEL_SENSORTHREAD_HPP_
