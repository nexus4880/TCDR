#pragma once

#include <numbers>
#include <optional>

class ProfileSettings {
public:
	ProfileSettings(uint64_t address) {
		this->address = address;
	}

	int GetRole();

private:
	uint64_t address;
	std::optional<int> cachedRole = std::nullopt;
};