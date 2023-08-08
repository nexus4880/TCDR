#include "Player.hpp"
#include "Global.hpp"
#include <xmmintrin.h>  
#include <emmintrin.h>
#include "Utils.h"
#include "Offsets.hpp"
#include "Hacks.hpp"

Player::Player(uint64_t address) :
	address(address), cachedEFTPlayerClassAddress(0),
	cachedTransformAddress(0)
{
}

ProfileInfo Player::GetProfileInfo() {
	if (!this->cachedProfileInfo.has_value()) {
		this->cachedProfileInfo = ProfileInfo{
			Memory::ReadChain<uint64_t>(
				Global::pMemoryInterface,
				this->address,
				{0x520, 0x28}
			)
		};
	}

	return this->cachedProfileInfo.value();
}

uint64_t Player::GetSkeletonTransformListValues() {
	if (!this->cachedEFTPlayerClassAddress) {
		this->cachedEFTPlayerClassAddress = Memory::ReadChain<uint64_t>(
			Global::pMemoryInterface,
			this->address,
			{ 0xA8, 0x28, 0x28, 0x10 }
		);
	}

	return this->cachedEFTPlayerClassAddress;
}


Vector3 Player::GetPosition() {
	if (!this->cachedTransformAddress) {
		this->cachedTransformAddress = Memory::ReadChain<uint64_t>(
			Global::pMemoryInterface,
			this->GetSkeletonTransformListValues(),
			{ 0x20, 0x10 }
		);
	}

	return this->cachedTransformAddress ? Hacks::ReadPosition(this->cachedTransformAddress) : Vector3{ 0.f, 0.f, 0.f };
}

Vector3 Player::GetBone(EBone bone) {
	uint64_t boneAddress;
	if (this->cachedBones.contains(bone)) {
		boneAddress = this->cachedBones[bone];
	}
	else {
		boneAddress = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, Memory::ReadValue<uint64_t>(Global::pMemoryInterface, this->GetSkeletonTransformListValues() + 0x20 + bone * 0x8) + 0x10);
		this->cachedBones[bone] = boneAddress;
	}

	return Hacks::ReadPosition(boneAddress);
}

void Player::DrawBones(float alpha, ProfileInfo& localPlayerInfo) {
}

InventoryController Player::GetInventoryController() {
	if (!this->cachedInventoryController.has_value()) {
		this->cachedInventoryController = InventoryController{
			Memory::ReadValue<uint64_t>(
				Global::pMemoryInterface,
				this->address + Offsets::Player::InventoryController
			)
		};
	}

	return this->cachedInventoryController.value();
}

FirearmController Player::GetFirearmController() {
	return FirearmController{ Memory::ReadValue<uint64_t>(Global::pMemoryInterface, this->address + 0x570) };
}
