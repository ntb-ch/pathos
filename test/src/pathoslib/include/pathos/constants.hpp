#ifndef CH_NTB_PATHOSLIB_CONSTANTS_HPP_
#define CH_NTB_PATHOSLIB_CONSTANTS_HPP_

namespace pathos {
	// DACs offsets
	static constexpr double dacResolution = 65535;
	
	static constexpr double dac1min = -9.817;
	static constexpr double dac1max =  9.678;
	static constexpr double dac1lsbs = (dac1max - dac1min) / (64000.0 - 2000);
	static constexpr double x1min = 2000 - (dac1min + 10.0) / dac1lsbs;
	static constexpr double x1max = 64000 - (dac1max - 10.0) / dac1lsbs;
	static constexpr double m1 = (x1max - x1min) / (2 * 10.0);
	static constexpr double dac1Scale  = 1.0 / m1;
	static constexpr double dac1Offset = -(10.0 + x1min / m1);
	
	static constexpr double dac2min = -9.784;
	static constexpr double dac2max =  9.814;
	static constexpr double dac2lsbs = (dac2max - dac2min) / (64000.0 - 2000);
	static constexpr double x2min = 2000 - (dac2min + 10.0) / dac2lsbs;
	static constexpr double x2max = 64000 - (dac2max - 10.0) / dac2lsbs;
	static constexpr double m2 = (x2max - x2min) / (2 * 10.0);
	static constexpr double dac2Scale  = 1.0 / m2;
	static constexpr double dac2Offset = - (10.0 + x2min / m2);
	
	static constexpr double dac3min = -9.667;
	static constexpr double dac3max =  9.925;
	static constexpr double dac3lsbs = (dac3max - dac3min) / (64000.0 - 2000);
	static constexpr double x3min = 2000 - (dac3min + 10.0) / dac3lsbs;
	static constexpr double x3max = 64000 - (dac3max - 10.0) / dac3lsbs;
	static constexpr double m3 = (x3max - x3min) / (2 * 10.0);
	static constexpr double dac3Scale  = 1.0 / m3;
	static constexpr double dac3Offset = - (10.0 + x3min / m3);
	
	static constexpr double dac4min = -9.759;
	static constexpr double dac4max =  9.885;
	static constexpr double dac4lsbs = (dac4max - dac4min) / (64000.0 - 2000);
	static constexpr double x4min = 2000 - (dac4min + 10.0) / dac4lsbs;
	static constexpr double x4max = 64000 - (dac4max - 10.0) / dac4lsbs;
	static constexpr double m4 = (x4max - x4min) / (2 * 10.0);
	static constexpr double dac4Scale  = 1.0 / m4;
	static constexpr double dac4Offset = - (10.0 + x4min / m4);
	
	static constexpr double enc1Scale = (2.0 * 3.14159) / (4.0 * 2048.0);
	static constexpr double enc2Scale = (2.0 * 3.14159) / (4.0 * 2048.0);
	static constexpr double enc3Scale = (2.0 * 3.14159) / (4.0 * 2048.0);
	static constexpr double enc4Scale = (2.0 * 3.14159) / (4.0 * 2048.0);
	
	static constexpr double enc1Offset = 0.0;
	static constexpr double enc2Offset = 0.0;
	static constexpr double enc3Offset = 0.0;
	static constexpr double enc4Offset = 0.0;
	
}
#endif