#include <iostream>
#include "IMemoryInterface.h"
#include "BasicMemoryInterface.h"
#include "TestOverlay.h"
#include "DriverInterop.h"
#include "Global.hpp"
#include <numeric>

#define USE_DRIVER 0

#ifdef _DEBUG
int main()
#else
unsigned long WinMain()
#endif
{
#if !defined(_DEBUG) || USE_DRIVER
	printf_s("Attempting to interop with driver\n");
	DriverInterop driverInterop{};
	Global::pMemoryInterface = &driverInterop;
#else
	printf_s("Attempting to use basic memory interface\n");
	BasicMemoryInterface basicMemoryInterface{};
	Global::pMemoryInterface = &basicMemoryInterface;
#endif

	if (!Global::pMemoryInterface->UpdateProcessId(L"EscapeFromTarkov.exe")) {
		throw;
	}

	Global::gom = GameObjectManager::Get();
	if (!Global::gom.address) {
		throw;
	}

	std::cout << "Press enter while in a game" << std::endl;
	std::cin.get();
	GameWorld gameWorld = GameWorld::Get();
	if (!gameWorld.address) {
		throw;
	}

	std::vector<Player>& players = gameWorld.GetPlayers();
	for (Player& player : players) {
		printf_s("%llx: %ws\n", player.address, player.GetProfileInfo().GetNickname().c_str());
	}

	std::cout << "EOF" << std::endl;
	std::cin.get();
}