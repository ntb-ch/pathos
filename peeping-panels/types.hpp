#ifndef CH_NTB_PARALLELSCARA_TYPES_HPP
#define CH_NTB_PARALLELSCARA_TYPES_HPP

#include <eeros/math/Matrix.hpp>
#include <array>
#include "constants.hpp"

namespace pathos {
	namespace peepingpanel {
		using AxisVector = eeros::math::Matrix<nofAxis, 1>;
		using AxisSquareMatrix = eeros::math::Matrix<nofAxis, nofAxis>;
		
		using PanelConfigArray  = std::array<double, 4>;
		using AllConfigArray    = std::array<double,17>; 
		using SensorsAreasArray = std::array<int,60>;
		
	}
}

#endif /* CH_NTB_PARALLELSCARA_TYPES_HPP */