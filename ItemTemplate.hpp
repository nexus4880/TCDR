#pragma once

#include <numbers>
#include <string>
#include <optional>

class ItemTemplate {
public:
	ItemTemplate(uint64_t address) {
		this->address = address;
	}

	std::wstring GetId();
	std::wstring GetLocalizedName();

	uint64_t address;

private:
	std::optional<std::wstring> cachedId;
	std::optional<std::wstring> cachedLocalizedName;
};