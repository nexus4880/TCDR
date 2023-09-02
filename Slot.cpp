#include "IMemoryInterface.h"
#include "Slot.hpp"
#include "Global.hpp"

Item Slot::GetContainedItem() {
	if (!this->cachedContainedItem.has_value()) {
		this->cachedContainedItem = Item{Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x38)};
	}

	return this->cachedContainedItem.value();
}

bool Slot::IsRequired() {
	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + 0x50);
}
