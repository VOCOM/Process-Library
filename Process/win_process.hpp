#ifndef WIN_PROCESS
#define WIN_PROCESS

#include <Windows.h>

class WindowsProcess {
public:
	WindowsProcess(const char* exeName);
	~WindowsProcess();

private:
	HANDLE handle;
	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION processInfo;
	DWORD exitCode;
};

#endif /* WIN_PROCESS */
