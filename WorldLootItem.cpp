#include "IMemoryInterface.h"
#include "Global.hpp"
#include "WorldLootItem.hpp"

bool WorldLootItem::IsContainer() {
	if (!this->cachedIsContainer.has_value()) {
		this->cachedIsContainer = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x18) == 0;
	}

	return this->cachedIsContainer.value();
}

Vector3 WorldLootItem::GetPosition() {
	if (!this->cachedTransformAddress) {
		this->cachedTransformAddress = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x10);
	}

	return Memory::ReadValue<Vector3>(Global::pMemoryInterface, this->cachedTransformAddress + 0xA90);
}

std::optional<std::wstring> WorldLootItem::GetId() {
	if (!this->cachedId.has_value()) {
		std::wstring readString = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x68));
		if (IS_VALID_WSTRING(readString)) {
			this->cachedId = readString;
		}
		else {
			this->cachedId = std::nullopt;
		}
	}

	return this->cachedId;
}

std::optional<std::wstring> WorldLootItem::GetLocalizedName() {
	if (!this->cachedLocalizedName.has_value()) {
		if (this->GetId().has_value()) {
			this->cachedLocalizedName = Global::itemTemplates[this->GetId().value()];
		}
	}

	return this->cachedLocalizedName;
}
