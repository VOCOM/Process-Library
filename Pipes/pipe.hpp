#ifndef PIPE
#define PIPE

#include <atomic>
#include <string>

class Pipe {
public:
	virtual void Start(const char* name) = 0;

	Pipe& operator>>(std::string& msg);
	Pipe& operator<<(const std::string& msg);

	friend std::istream& operator>>(std::istream& is, Pipe& pipe);
	friend std::ostream& operator<<(std::ostream& os, Pipe& pipe);

	virtual ~Pipe() {}

	static const unsigned long BUFFER_SIZE = 0xFFF;

protected:
	std::string inBuffer;
	std::string outBuffer;
	std::atomic_bool lock;
};

#endif /* PIPE */
