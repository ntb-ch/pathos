#ifndef ORG_PATHOS_DECOY_RADTOPULSES_HPP_
#define ORG_PATHOS_DECOY_RADTOPULSES_HPP_

#include <eeros/math/Matrix.hpp>
#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Output.hpp>
#include <vector>

namespace pathos {
	namespace decoy {
		
		class RadToPulses: public eeros::control::Block {
			
		public:
			RadToPulses(double enc_pulses, double i_gear);
			virtual ~RadToPulses();
			virtual void run();
			
			virtual eeros::control::Input<double>& getIn();
			virtual eeros::control::Output<int>& getOut();
			
		protected:
			eeros::control::Input<double> in;
			eeros::control::Output<int> out;
			
			double encPulses, gear;

		};
	};
};

#endif /* ORG_PATHOS_DECOY_RADTOPULSES_HPP_ */
