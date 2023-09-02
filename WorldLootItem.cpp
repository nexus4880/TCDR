#include "IMemoryInterface.h"
#include "Global.hpp"
#include "WorldLootItem.hpp"
#include "Hacks.hpp"

Vector3 WorldLootItem::GetPosition() {
	if (!this->cachedTransform.has_value()) {
		this->cachedTransform = Memory::ReadChain<uintptr_t>(Global::pMemoryInterface, this->address, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
	}

	return Hacks::ReadPosition(this->cachedTransform.value());
}

std::wstring WorldLootItem::GetId() {
	if (!this->cachedId.has_value()) {
		this->cachedId = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x68));
	}

	return this->cachedId.value();
}

std::wstring WorldLootItem::GetLocalizedName(bool* isLocalized) {
	if (!this->cachedLocalizedName.has_value()) {
		std::wstring id = this->GetId();
		if (Global::itemTemplates.contains(id)) {
			if (isLocalized) {
				*isLocalized = true;
			}

			this->cachedLocalizedName = Global::itemTemplates[id];
		}
		else {
			if (isLocalized) {
				*isLocalized = false;
			}

			this->cachedLocalizedName = UNLOCALIZED_ITEM;
		}
	}

	return this->cachedLocalizedName.value();
}