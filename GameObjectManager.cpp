#include "GameObjectManager.hpp"
#include "Offsets.hpp"
#include "Global.hpp"

GameObjectManager GameObjectManager::Get() {
	return GameObjectManager{Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, Global::pMemoryInterface->GetModuleBase() + Offsets::GOM)};
}

ListNode GameObjectManager::GetActiveObjects() const {
	return Memory::ReadValue<ListNode>(Global::pMemoryInterface, this->address + Offsets::GameObjectManager::ActiveObjects);
}

ListNode GameObjectManager::GetTaggedObjects() const {
	return Memory::ReadValue<ListNode>(Global::pMemoryInterface, this->address + Offsets::GameObjectManager::TaggedObjects);
}

ListNode GameObjectManager::GetCameraList() const {
	return Memory::ReadValue<ListNode>(Global::pMemoryInterface, this->address + 0x10);
}
