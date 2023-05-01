#include "IMemoryInterface.h"
#include "Global.hpp"
#include "ItemTemplate.hpp"

wchar_t* ItemTemplate::GetId() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x50));
}

wchar_t* ItemTemplate::GetName() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x10));
}

wchar_t* ItemTemplate::GetShortName() {
	return Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x18));
}

bool ItemTemplate::IsQuestItem() {
	return Memory::ReadValue<bool>(Global::pMemoryInterface, this->address + 0x9C);
}
