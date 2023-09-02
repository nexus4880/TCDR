#include "IMemoryInterface.h"
#include "FirearmController.hpp"
#include "Global.hpp"

Item FirearmController::GetItem() {
	return Item{ Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x60) };
}

bool FirearmController::IsAiming() {
	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + 0x169);
}
