#pragma once

#include <numbers>
#include <optional>

class ProfileSettings {
public:
	ProfileSettings(intptr_t address) {
		this->address = address;
	}

	int GetRole();

private:
	intptr_t address;
	std::optional<int> cachedRole = std::nullopt;
};