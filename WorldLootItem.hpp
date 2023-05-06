#pragma once

#include <numbers>
#include <optional>
#include <raylib.h>
#include <string>

class WorldLootItem {
public:
	WorldLootItem(intptr_t address) : cachedTransformAddress(0) {
		this->address = address;
	}

	bool IsContainer();
	Vector3 GetPosition();
	std::optional<std::wstring> GetId();
	std::optional<std::wstring> GetLocalizedName();

	intptr_t address;
	
private:
	std::optional<bool> cachedIsContainer = std::nullopt;
	intptr_t cachedTransformAddress;
	std::optional<std::wstring> cachedId;
	std::optional<std::wstring> cachedLocalizedName;
};