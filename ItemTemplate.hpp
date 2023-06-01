#pragma once

#include <numbers>
#include <string>
#include <optional>

class ItemTemplate {
public:
	ItemTemplate(intptr_t address) {
		this->address = address;
	}

	std::wstring GetId();
	std::wstring GetLocalizedName();

	intptr_t address;

private:
	std::optional<std::wstring> cachedId;
	std::optional<std::wstring> cachedLocalizedName;
};