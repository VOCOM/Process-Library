#include <Pipes/pipe_server.hpp>

#include <iostream>

bool PipeServer::Connected() const { return connected; }

void PipeServer::Start(const char* name) {
	pipeName += name;
	pThread = std::thread(&PipeServer::Loop, this);
}

// PipeServer& PipeServer::operator>>(std::string& msg) {
// 	while (!outBuffer.size() || lock) std::this_thread::yield();
// 	lock = true;
// 	msg  = outBuffer;
// 	outBuffer.clear();
// 	lock = false;
// 	return *this;
// }
// PipeServer& PipeServer::operator<<(const std::string& msg) {
// 	while (lock) std::this_thread::yield();
// 	lock = true;
// 	inBuffer += msg;
// 	lock = false;
// 	return *this;
// }

// std::istream& operator>>(std::istream& is, PipeServer& pipe) {
// 	std::string value;
// 	is >> value;
// 	pipe << value;
// 	return is;
// }
// std::ostream& operator<<(std::ostream& os, PipeServer& pipe) {
// 	std::string value;
// 	pipe >> value;
// 	os << value;
// 	return os;
// }

PipeServer::PipeServer() : pipeName("\\\\.\\pipe\\") {}
PipeServer::~PipeServer() {
	connected = false;
	CloseHandle(pHandle);
	if (pThread.joinable()) pThread.join();
}

void PipeServer::Loop() {
	pHandle = CreateNamedPipeA(
			pipeName.c_str(),                                                                   // Pipe name
			PIPE_ACCESS_DUPLEX,                                                                 // Pipe direction | blocking
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS, // Message type
			PIPE_UNLIMITED_INSTANCES,                                                           // Max pipe size
			BUFFER_SIZE * sizeof(CHAR),                                                         // Output buffer size
			BUFFER_SIZE * sizeof(CHAR),                                                         // Input buffer size
			NULL,                                                                               // Timeout
			NULL                                                                                // Security
	);
	connected = ConnectNamedPipe(pHandle, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	if (connected) std::cout << "Master Connected.\n";

	char buffer[BUFFER_SIZE];
	DWORD readCount, writeCount;
	while (connected) {
		while (lock) std::this_thread::yield();
		lock = true;

		connected = WriteFile(pHandle, (LPVOID)inBuffer.data(), (DWORD)inBuffer.size(), &writeCount, NULL);
		if (writeCount) inBuffer.clear();

		connected = ReadFile(pHandle, buffer, BUFFER_SIZE, &readCount, NULL);
		if (readCount) outBuffer.append(buffer, readCount);

		lock = false;
	}
	CloseHandle(pHandle);
}
