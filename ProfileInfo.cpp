#include "ProfileInfo.hpp"
#include "Global.hpp"
#include "Utils.h"

unsigned char ProfileInfo::GetSide() {
	if (!this->cachedSide.has_value()) {
		this->cachedSide = Memory::ReadValue<unsigned char>(Global::pMemoryInterface, this->address + 0x70);
	}

	return this->cachedSide.value();
}

int ProfileInfo::GetRegistrationDate() {
	if (!this->cachedRegistrationDate.has_value()) {
		this->cachedRegistrationDate = Memory::ReadValue<int>(Global::pMemoryInterface, this->address + 0x74);
	}

	return this->cachedRegistrationDate.value();
}

std::wstring ProfileInfo::GetNickname() {
	if (!this->cachedNickname.has_value()) {
		this->cachedNickname = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x10));
	}

	return this->cachedNickname.value();
}

std::wstring ProfileInfo::GetGroupID() {
	if (!this->cachedGroupID.has_value()) {
		this->cachedGroupID = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x20));
	}

	return this->cachedGroupID.value();
}

std::wstring ProfileInfo::GetGroupHash() {
	if (!this->cachedGroupHash.has_value()) {
		std::wstring id = this->GetGroupID();
		if (IS_VALID_WSTRING(id)) {
			std::string idStr(id.begin(), id.end());
			unsigned int hash = Utils::HashString(idStr);
			id = std::to_wstring(hash % 100000);
		}
		else {
			id = NULL_WSTRING;
		}

		this->cachedGroupHash = id;
	}

	return this->cachedGroupHash.value();
}

ProfileSettings ProfileInfo::GetSettings() {
	if (!this->cachedProfileSettings.has_value()) {
		this->cachedProfileSettings = ProfileSettings{Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x50)};
	}

	return this->cachedProfileSettings.value();
}

bool ProfileInfo::IsPlayer() {
	return this->GetSide() != 4 || this->GetRegistrationDate() != 0;
}

bool ProfileInfo::IsBoss() {
	if (this->GetSide() != 4) {
		return false;
	}

	switch (this->GetSettings().GetRole()) {
		case 2:
		case 3:
		case 6:
		case 7:
		case 11:
		case 17:
		case 22:
		case 26:
		case 27:
		case 28:
		case 29:
			return true;
		default:
			return false;
	}

	return this->GetSide() == 4;
}

EMemberCategory ProfileInfo::GetMemberCategory() {
	if (!this->cachedMemberCategory.has_value()) {
		this->cachedMemberCategory = static_cast<EMemberCategory>(Memory::ReadValue<int>(Global::pMemoryInterface, this->address + 0x8C));
	}

	return this->cachedMemberCategory.value();
}
