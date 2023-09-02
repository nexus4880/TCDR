#include "InventoryController.hpp"
#include "IMemoryInterface.h"
#include "Global.hpp"
#include "Offsets.hpp"

bool InventoryController::IsInInventory() {
	return false;

	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + Offsets::InventoryController::IsInInventory) == false;
}

Slot InventoryController::GetSlot(EquipmentSlot slot) {
	if (!this->cachedSlots[slot].has_value()) {
		this->cachedSlots[slot] = Slot{
			Memory::ReadValue<uintptr_t>(
				Global::pMemoryInterface,
				Memory::ReadChain<uintptr_t>(
					Global::pMemoryInterface, this->address, {Offsets::InventoryController::InventoryClass, Offsets::InventoryClass::EquipmentClass, 0x78}
				) + Offsets::List::FirstEntry + sizeof(uintptr_t) * slot
			)
		};
	}

	return this->cachedSlots[slot].value();
}
