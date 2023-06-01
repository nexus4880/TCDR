#include "IMemoryInterface.h"
#include "Global.hpp"
#include "ItemTemplate.hpp"

std::wstring ItemTemplate::GetId() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x50));
}

std::wstring ItemTemplate::GetName() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x10));
}

std::wstring ItemTemplate::GetShortName() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x18));
}

bool ItemTemplate::IsQuestItem() {
	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + 0x9C);
}
