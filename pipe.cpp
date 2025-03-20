#include <Pipes/pipe.hpp>

#include <thread>

Pipe& Pipe::operator>>(std::string& msg) {
	while (!outBuffer.size() || lock) std::this_thread::yield();
	lock = true;
	msg  = outBuffer;
	outBuffer.clear();
	lock = false;
	return *this;
}
Pipe& Pipe::operator<<(const std::string& msg) {
	while (lock) std::this_thread::yield();
	lock = true;
	inBuffer += msg;
	lock = false;
	return *this;
}

std::istream& operator>>(std::istream& is, Pipe& pipe) {
	std::string value;
	is >> value;
	pipe << value;
	return is;
}
std::ostream& operator<<(std::ostream& os, Pipe& pipe) {
	std::string value;
	pipe >> value;
	os << value;
	return os;
}
