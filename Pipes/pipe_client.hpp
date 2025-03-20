#ifndef PIPE_CLIENT
#define PIPE_CLIENT

#include <atomic>
#include <string>
#include <thread>

#include <Windows.h>

class PipeClient {
public:
	void Connect(const char* name);

	PipeClient& operator<<(const std::string& msg);
	PipeClient& operator>>(std::string& msg);

	friend std::istream& operator>>(std::istream& is, PipeClient& pipe);
	friend std::ostream& operator<<(std::ostream& os, PipeClient& pipe);

	PipeClient();
	~PipeClient();

private:
	void Loop();

	bool connected;
	HANDLE pHandle;
	std::thread pThread;
	std::string pipeName;

	std::string inBuffer;
	std::string outBuffer;
	std::atomic_bool lock;

	static const unsigned long BUFFER_SIZE = 0xFFF;
};

#endif /* PIPE_CLIENT */
