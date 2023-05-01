#include <iostream>
#include "IMemoryInterface.h"
#include "BasicMemoryInterface.h"
#include "TestOverlay.h"
#include "DriverInterop.h"
#include "Global.hpp"

#define USE_DRIVER 0

#ifdef _DEBUG
int main()
#else
unsigned long WinMain()
#endif
{
#if USE_DRIVER
	printf_s("Attempting to interop with driver\n");
	DriverInterop driverInterop{};
	Global::pMemoryInterface = &driverInterop;
#else
	printf_s("Attempting to use basic memory interface\n");
	BasicMemoryInterface basicMemoryInterface{};
	Global::pMemoryInterface = &basicMemoryInterface;
#endif
	TestOverlay overlay{"TestOverlay", 60};
	while (overlay.IsRunning()) {
		overlay.Update();
		overlay.Draw();
	}
}