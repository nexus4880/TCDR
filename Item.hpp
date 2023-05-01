#pragma once

#include <numbers>
#include "ItemTemplate.hpp"
#include <optional>

class Item {
public:
	Item(intptr_t address) {
		this->address = address;
	}

	int GetVersion();
	bool IsUnlimited();
	ItemTemplate GetTemplate();
	
	intptr_t address;
	
private:
	std::optional<ItemTemplate> cachedTemplate = std::nullopt;
};
