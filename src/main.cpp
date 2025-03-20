#include <iostream>
#include <string>

#include <Pipes/pipes.hpp>
#include <Process/process.hpp>

using namespace std::chrono_literals;

int main(int argc, char** argv) {
	WindowsProcess subProcess("CMP_sub.exe");

	PipeServer subPipe;
	subPipe.Start("sub");

	std::string result;
	while (subPipe.Connected()) {
		std::cin >> subPipe;
		subPipe >> result;
		std::cout << "Master received " << result << "\n";
		if (result == "exit") break;
	}

	std::cout << "Master exiting...\n";
}
