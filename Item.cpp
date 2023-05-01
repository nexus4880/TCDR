#include "Item.hpp"
#include "IMemoryInterface.h"
#include "Global.hpp"

int Item::GetVersion() {
	return Memory::ReadValue<int>(Global::pMemoryInterface, this->address + 0x68);
}

bool Item::IsUnlimited() {
	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + 0x58);
}

ItemTemplate Item::GetTemplate() {
	if (!this->cachedTemplate.has_value()) {
		this->cachedTemplate = ItemTemplate{
			Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x40)
		};
	}

	return this->cachedTemplate.value();
}
