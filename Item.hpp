#pragma once

#include <numbers>
#include "ItemTemplate.hpp"
#include <optional>

class Item {
public:
	Item(uint64_t address) {
		this->address = address;
	}

	int GetVersion();
	bool IsUnlimited();
	ItemTemplate GetTemplate();
	
	uint64_t address;
	
private:
	std::optional<ItemTemplate> cachedTemplate = std::nullopt;
};
