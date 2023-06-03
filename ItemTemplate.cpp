#include "IMemoryInterface.h"
#include "Global.hpp"
#include "ItemTemplate.hpp"

std::wstring ItemTemplate::GetId() {
	if (!this->cachedId.has_value()) {
		this->cachedId = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<uint64_t>(Global::pMemoryInterface, this->address + 0x50));
	}

	return this->cachedId.value();
}

std::wstring ItemTemplate::GetLocalizedName() {
	if (!this->cachedLocalizedName.has_value()) {
		std::wstring templateId = this->GetId();
		if (Global::itemTemplates.contains(templateId)) {
			this->cachedLocalizedName = Global::itemTemplates[templateId];
		}
		else {
			this->cachedLocalizedName = L"unlocalized_weapon";
		}
	}

	return this->cachedLocalizedName.value();
}