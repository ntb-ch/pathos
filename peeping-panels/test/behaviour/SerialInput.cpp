#include "SerialInput.hpp"

using namespace pathos::peepingpanel;

Port::Port(const char *file) {
	fd = ::open(file, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) throw std::runtime_error("cannot open file");
	block(false);
}

Port::~Port() {
	::close(fd);
}

void Port::block(bool enable) {
	if (enable)
		::fcntl(fd, F_SETFL, 0);
	else
		::fcntl(fd, F_SETFL, FNDELAY);
}

void Port::baud(Baud value) {
	speed_t speed = 0;

	switch (value) {
		case Baud::b9600:   speed = B9600; break;
		case Baud::b19200:  speed = B19200; break;
		case Baud::b38400:  speed = B38400; break;
		case Baud::b57600:  speed = B57600; break;
		case Baud::b115200: speed = B115200; break;
		default: throw std::runtime_error("unsupported baud rate");
	}

	struct termios t;
	if (tcgetattr(fd, &t) != 0)
		throw std::runtime_error("cannot get terminal option");

	::cfsetispeed(&t, speed);
	::cfsetospeed(&t, speed);

	t.c_cflag |= (CLOCAL | CREAD);

	if (tcsetattr(fd, TCSANOW, &t) != 0)
		throw std::runtime_error("cannot set terminal option");
}

int Port::write(const char *data, int offset, int length) {
	return ::write(fd, data + offset, length);
}

int Port::write(std::string s) {
	return write(s.c_str(), 0, s.length());
}

int Port::read(char *buffer, int size) {
	return ::read(fd, buffer, size);
}