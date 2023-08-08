#pragma once

#include <numbers>
#include "Slot.hpp"
#include "EquipmentSlot.hpp"
#include <optional>

class InventoryController {
public:
	InventoryController(uint64_t address) {
		this->address = address;
	}

	Slot GetSlot(EquipmentSlot slot);

	uint64_t address;

private:
	bool hasReadAll = false;
	std::optional<Slot> cachedSlots[15];
};