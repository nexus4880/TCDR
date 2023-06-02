#pragma once

#include <numbers>
#include <optional>
#include <raylib.h>
#include <string>

const std::wstring UNLOCALIZED_ITEM = L"unlocalized_item";

class WorldLootItem {
public:
	WorldLootItem(uint64_t address) {
		this->address = address;
	}

	Vector3 GetPosition();
	std::wstring GetId();
	std::wstring GetLocalizedName(bool* isLocalized);

	uint64_t address;
	
private:
	std::optional<uint64_t> cachedTransform;
	std::optional<std::wstring> cachedId;
	std::optional<std::wstring> cachedLocalizedName;
};