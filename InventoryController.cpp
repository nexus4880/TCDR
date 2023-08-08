#include "InventoryController.hpp"
#include "IMemoryInterface.h"
#include "Global.hpp"
#include "Offsets.hpp"

Slot InventoryController::GetSlot(EquipmentSlot slot) {
	if (!this->cachedSlots[slot].has_value()) {
		this->cachedSlots[slot] = Slot{
			Memory::ReadValue<uint64_t>(
				Global::pMemoryInterface,
				Memory::ReadChain<uint64_t>(
					Global::pMemoryInterface, this->address, {Offsets::InventoryController::InventoryClass, Offsets::InventoryClass::EquipmentClass, 0x78}
				) + Offsets::List::FirstEntry + sizeof(uint64_t) * slot
			)
		};
	}

	return this->cachedSlots[slot].value();
}
