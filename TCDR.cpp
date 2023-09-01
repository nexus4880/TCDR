#include <iostream>
#include "IMemoryInterface.h"
#include "BasicMemoryInterface.h"
#include "DMAInterop.hpp"
#include "TestOverlay.h"
#include "DriverInterop.h"
#include "Global.hpp"
#include <numeric>

#define MEMORY_INTERFACE_MODE 2

#ifdef _DEBUG
int main()
#else
unsigned long WinMain()
#endif
{
	try {
#if (MEMORY_INTERFACE_MODE == 0)
		printf_s("Attempting to use basic memory interface\n");
		BasicMemoryInterface basicMemoryInterface{};
		Global::pMemoryInterface = &basicMemoryInterface;
#elif MEMORY_INTERFACE_MODE == 1
		printf_s("Attempting to interop with driver\n");
		DriverInterop driverInterop{};
		Global::pMemoryInterface = &driverInterop;
#elif MEMORY_INTERFACE_MODE == 2
		printf_s("Attempting to use DMA interop\n");
		DMAInterop dmaInterop{};
		Global::pMemoryInterface = &dmaInterop;
#endif
	}
	catch (const std::exception& ex) {
		printf_s(ex.what());

		std::cin.get();

		return 1;
	}

	TestOverlay overlay{"TestOverlay", 60};
	while (overlay.IsRunning()) {
		overlay.Update();
		overlay.Draw();
	}
}