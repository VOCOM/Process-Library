#include <Pipes/pipe_client.hpp>

#include <iostream>

void PipeClient::Connect(const char* name) {
	pipeName += name;
	pThread = std::thread(&PipeClient::Loop, this);
}

PipeClient& PipeClient::operator>>(std::string& msg) {
	if (!outBuffer.size()) {
		msg.clear();
	} else {
		while (lock) std::this_thread::yield();
		lock = true;
		msg  = outBuffer;
		outBuffer.clear();
		lock = false;
	}
	return *this;
}
PipeClient& PipeClient::operator<<(const std::string& msg) {
	while (lock) std::this_thread::yield();
	lock = true;
	inBuffer += msg;
	lock = false;
	return *this;
}

std::istream& operator>>(std::istream& is, PipeClient& pipe) {
	std::string value;
	is >> value;
	pipe << value;
	return is;
}
std::ostream& operator<<(std::ostream& os, PipeClient& pipe) {
	std::string value;
	pipe >> value;
	os << value;
	return os;
}

PipeClient::PipeClient() : pipeName("\\\\.\\pipe\\") {}
PipeClient::~PipeClient() {
	connected = false;
	CloseHandle(pHandle);
	if (pThread.joinable()) pThread.join();
	std::cout << "Closed Client\n";
}

void PipeClient::Loop() {
	while (!WaitNamedPipeA(pipeName.c_str(), NMPWAIT_WAIT_FOREVER)) std::this_thread::yield();
	pHandle = CreateFileA(
			pipeName.c_str(),             // pipe name
			GENERIC_READ | GENERIC_WRITE, // read and write access
			NULL,                         // no sharing
			NULL,                         // default security attributes
			OPEN_EXISTING,                // opens existing pipe
			NULL,                         // default attributes
			NULL);                        // no template file
	if (pHandle != INVALID_HANDLE_VALUE) std::cout << "Client Connected.\n";
	DWORD newMode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(pHandle, &newMode, NULL, NULL);

	char buffer[BUFFER_SIZE]{};
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
