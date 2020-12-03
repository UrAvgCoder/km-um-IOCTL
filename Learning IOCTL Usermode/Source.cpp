#include <iostream>
#include <Windows.h>
#include "cheat.hpp"

int main()
{

	std::cout << "[kernel-csgo] Initializing.." << std::endl;
	std::cout << "[kernel-csgo] Initialization complete.." << std::endl;
	cheat::run();

	std::cin.get();
	return 0;
}

