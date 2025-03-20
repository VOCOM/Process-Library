#ifndef PIPE_SERVER
#define PIPE_SERVER

#include <Windows.h>

#include <Pipes/pipe.hpp>

#include <thread>

class PipeServer : public Pipe {
public:
	bool Connected() const;
	virtual void Start(const char* name);

	PipeServer();
	~PipeServer();

private:
	void Loop();

	bool connected;
	HANDLE pHandle;
	std::thread pThread;
	std::string pipeName;
};

// class PipeServer {
// public:
// 	bool Connected() const;
// 	void Start(const char* name);

// 	PipeServer& operator>>(std::string& msg);
// 	PipeServer& operator<<(const std::string& msg);

// 	friend std::istream& operator>>(std::istream& is, PipeServer& pipe);
// 	friend std::ostream& operator<<(std::ostream& os, PipeServer& pipe);

// 	PipeServer();
// 	~PipeServer();

// private:
// 	void Loop();

// 	bool connected;
// 	HANDLE pHandle;
// 	std::thread pThread;
// 	std::string pipeName;

// 	std::string inBuffer;
// 	std::string outBuffer;
// 	std::atomic_bool lock;

// 	static const unsigned long BUFFER_SIZE = 0xFFF;
// };

#endif /* PIPE_SERVER */
