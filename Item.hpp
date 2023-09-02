#pragma once

#include <numbers>
#include "ItemTemplate.hpp"
#include <optional>

class Item {
public:
	Item(uintptr_t address)
		: address(address)
	{
	}

	int GetVersion();
	bool IsUnlimited();
	ItemTemplate GetTemplate();
	
private:
	uintptr_t address;
	std::optional<ItemTemplate> cachedTemplate = std::nullopt;
};
