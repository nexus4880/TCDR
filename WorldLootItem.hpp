#pragma once

#include <numbers>
#include <optional>
#include <raylib.h>

class WorldLootItem {
public:
	WorldLootItem(intptr_t address) : cachedTransformAddress(0) {
		this->address = address;
	}

	bool IsContainer();
	Vector3 GetPosition();

	intptr_t address;
	
private:
	std::optional<bool> cachedIsContainer = std::nullopt;
	intptr_t cachedTransformAddress;
};