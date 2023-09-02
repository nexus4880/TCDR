#include <iostream>
#include "IMemoryInterface.h"
#include "TestOverlay.h"
#include "Global.hpp"
#include <numeric>

#ifdef MEMORY_INTERFACE_BASIC
	#include "BasicMemoryInterface.h"
#endif

#ifdef MEMORY_INTERFACE_DRIVER
	#include "DriverInterop.h"
#endif

#ifdef MEMORY_INTERFACE_DMA
	#include "DMAInterop.hpp"
#endif

#ifdef _DEBUG
	using ENTRY_RESULT = int;
	#define ENTRY_NAME main
#else
	using ENTRY_RESULT = unsigned long;
	#define ENTRY_NAME WinMain
#endif

ENTRY_RESULT ENTRY_NAME()
{
	try {
#ifdef MEMORY_INTERFACE_BASIC
		printf_s("Attempting to use basic memory interface\n");
		BasicMemoryInterface basicMemoryInterface{};
		Global::pMemoryInterface = &basicMemoryInterface;
#endif
#ifdef MEMORY_INTERFACE_DRIVER
		printf_s("Attempting to interop with driver\n");
		DriverInterop driverInterop{};
		Global::pMemoryInterface = &driverInterop;
#endif
#ifdef MEMORY_INTERFACE_DMA
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