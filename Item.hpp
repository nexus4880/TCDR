#pragma once

#include <numbers>
#include "ItemTemplate.hpp"
#include <optional>

class Item {
public:
	Item(uint64_t address)
		: address(address)
	{
	}

	int GetVersion();
	bool IsUnlimited();
	ItemTemplate GetTemplate();
	
private:
	uint64_t address;
	std::optional<ItemTemplate> cachedTemplate = std::nullopt;
};
