#pragma once

#include <numbers>
#include <optional>

class ProfileSettings {
public:
	ProfileSettings(uintptr_t address) {
		this->address = address;
	}

	int GetRole();

private:
	uintptr_t address;
	std::optional<int> cachedRole = std::nullopt;
};