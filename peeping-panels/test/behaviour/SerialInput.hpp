#ifndef CH_NTB_PEEPINGPANEL_SERIALINPUT_HPP_
#define CH_NTB_PEEPINGPANEL_SERIALINPUT_HPP_

#include <iostream>
#include <sstream>

#include <stdexcept>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

namespace pathos {
	namespace peepingpanel {
		
		enum class Baud {
			b9600,
			b19200,
			b38400,
			b57600,
			b115200
		};
		
		class Port {
			public:
				Port(const char *file);
				~Port();

				void block(bool enable = true);
				void baud(Baud value);

				int write(const char *data, int offset, int length);

				template < typename T, size_t N >
				int write(T (&data)[N]);
				
				int write(std::string s);

				int read(char *buffer, int size);

			private:
				int fd;
		};

		template < typename T, size_t N >
		int Port::write(T (&data)[N]) {
			return write(data, 0, N);
		}
		
	};
};

#endif /* CH_NTB_PEEPINGPANEL_SERIALINPUT_HPP_ */
