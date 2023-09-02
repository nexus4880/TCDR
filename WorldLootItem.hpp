#pragma once

#include <numbers>
#include <optional>
#include <raylib.h>
#include <string>

const std::wstring UNLOCALIZED_ITEM = L"unlocalized_item";

class WorldLootItem {
public:
	WorldLootItem(uintptr_t address) {
		this->address = address;
	}

	Vector3 GetPosition();
	std::wstring GetId();
	std::wstring GetLocalizedName(bool* isLocalized);

	uintptr_t address;
private:
	std::optional<uintptr_t> cachedTransform;
	std::optional<std::wstring> cachedId;
	std::optional<std::wstring> cachedLocalizedName;
};