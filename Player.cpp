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
	std::map<EBone, Vector3> boneWorldToScreens{};
	for (int i = 0; i < BONE_PATH_SIZE; i++) {
		std::array<EBone, 2> path = Player::drawPaths[i];
		Vector3 first;
		if (!boneWorldToScreens.contains(path[0])) {
			first = Global::activeCamera.WorldToScreen(this->GetBone(path[0]));
			boneWorldToScreens[path[0]] = first;
		}
		else {
			first = boneWorldToScreens[path[0]];
		}

		if (first.x == 0.f || first.y == 0.f || first.z <= 0.01f) {
			continue;
		}

		Vector3 second;
		if (!boneWorldToScreens.contains(path[1])) {
			second = Global::activeCamera.WorldToScreen(this->GetBone(path[1]));
			boneWorldToScreens[path[1]] = second;
		}
		else {
			second = boneWorldToScreens[path[1]];
		}

		if (second.x == 0.f || second.y == 0.f || second.z <= 0.01f) {
			continue;
		}
		DrawLine(static_cast<int>(first.x), static_cast<int>(first.y), static_cast<int>(second.x), static_cast<int>(second.y), ColorAlpha(this->GetColor(localPlayerInfo), alpha));
	}
}

Color Player::GetColor(ProfileInfo inRelationTo) {
	if (!this->cachedColor.has_value()) {
		ProfileInfo info = this->GetProfileInfo();
		const std::wstring playerGroupId = info.GetGroupID();
		const std::wstring relativeId = inRelationTo.GetGroupID();
		if (!IS_VALID_WSTRING(playerGroupId) || !IS_VALID_WSTRING(relativeId) || relativeId != playerGroupId) {
			switch (info.GetSide()) {
			case 1:
			{
				this->cachedColor = YELLOW;
				break;
			}
			case 2:
			{
				this->cachedColor = RED;
				break;
			}
			case 4:
			{
				this->cachedColor = info.IsPlayer() ? DARKBLUE : SKYBLUE;
				break;
			}
			}
		}
		else {
			this->cachedColor = GREEN;
		}
	}

	return cachedColor.value();
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

Item Player::GetActiveWeapon() {
	return Item{ Memory::ReadChain<uint64_t>(Global::pMemoryInterface, this->address, {0x570, 0x60}) };
}
