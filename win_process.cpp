#include <Process/win_process.hpp>

#include <iostream>

WindowsProcess::WindowsProcess(const char* exeName) {
	ZeroMemory(&processInfo, sizeof(processInfo));
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	CreateProcessA(
			exeName,      // Executable name
			NULL,         // Command line
			NULL,         // Process handle inheritance
			NULL,         // Thread handle inheritance
			TRUE,         // Set handle inheritance
			NULL,         // Creation flags
			NULL,         // Environment block
			NULL,         // Starting directory
			&startupInfo, // Startup info
			&processInfo  // Process information
	);
}
WindowsProcess::~WindowsProcess() {
	GetExitCodeProcess(processInfo.hProcess, &exitCode);
	if (exitCode) TerminateProcess(processInfo.hProcess, 1);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	std::cout << "Closed process.\n";
}
