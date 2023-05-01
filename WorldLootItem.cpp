#include "IMemoryInterface.h"
#include "Global.hpp"
#include "WorldLootItem.hpp"

bool WorldLootItem::IsContainer() {
	if (!this->cachedIsContainer.has_value()) {
		this->cachedIsContainer = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x18) == 0;
	}

	return this->cachedIsContainer.value();
}

Vector3 WorldLootItem::GetPosition() {
	if (!this->cachedTransformAddress) {
		this->cachedTransformAddress = Memory::ReadChain<intptr_t>(Global::pMemoryInterface, this->address, {0x10, 0x30, 0x28, 0x90});
	}

	return Memory::ReadValue<Vector3>(Global::pMemoryInterface, this->cachedTransformAddress + 0x3A0);
}
