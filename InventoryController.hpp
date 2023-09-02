#pragma once

#include <numbers>
#include "Slot.hpp"
#include "EquipmentSlot.hpp"
#include <optional>

class InventoryController {
public:
	InventoryController(uintptr_t address) {
		this->address = address;
	}

	bool IsInInventory();
	Slot GetSlot(EquipmentSlot slot);

	uintptr_t address;

private:
	bool hasReadAll = false;
	std::optional<Slot> cachedSlots[15];
};