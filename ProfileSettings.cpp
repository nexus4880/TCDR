#include "ProfileSettings.hpp"
#include "Global.hpp"
#include "Utils.h"

int ProfileSettings::GetRole() {
	if (!this->cachedRole.has_value()) {
		this->cachedRole = Memory::ReadValue<int>(Global::pMemoryInterface, this->address + 0x10);
	}

	return this->cachedRole.value();
}
