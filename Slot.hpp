#pragma once

#include <numbers>
#include "Item.hpp"
#include <optional>

class Slot {
public:
	Slot() : Slot(0) {
	}

	Slot(uint64_t address) {
		this->address = address;
	}

	Item GetContainedItem();
	bool IsRequired();

	uint64_t address;

private:
	std::optional<Item> cachedContainedItem = std::nullopt;
};