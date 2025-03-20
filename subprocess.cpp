#include <iostream>
#include <string>
#include <thread>

#include <Pipes/pipes.hpp>

int main(int argc, char** argv) {
	PipeClient subPipe;
	subPipe.Connect("sub");

	std::string result;
	while (true) {
		subPipe >> result;
		if (result.size()) std::cout << "Client received: " << result << "\n";
		if (result == "sexit") break;
	}
	std::cout << "Client exiting...\n";
	subPipe << "exit";
}
