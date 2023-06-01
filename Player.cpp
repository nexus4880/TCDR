#include "Player.hpp"
#include "Global.hpp"
#include <xmmintrin.h>  
#include <emmintrin.h>
#include "Utils.h"
#include "Offsets.hpp"

struct transform_access_read_only_t {
	uint64_t transform_data{};
	uint64_t data;
	int index;
};

struct transform_data_t {
	uint64_t transform_array{};
	uint64_t transform_indices{};
};

struct matrix34_t {
	Vector4 vec0{};
	Vector4 vec1{};
	Vector4 vec2{};
};

Vector3 GetPositionFromTransform(intptr_t transform) {
	__m128 result{};

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	transform_access_read_only_t pTransformAccessReadOnly = Memory::ReadValue<transform_access_read_only_t>(Global::pMemoryInterface, transform + 0x38);
	//pTransformAccessReadOnly.index
	unsigned int index = Memory::ReadValue<unsigned int>(Global::pMemoryInterface, transform + 0x40);
	transform_data_t transformData = Memory::ReadValue<transform_data_t>(Global::pMemoryInterface, pTransformAccessReadOnly.transform_data + 0x18);

	if (transformData.transform_array && transformData.transform_indices) {
		result = Memory::ReadValue<__m128>(Global::pMemoryInterface, transformData.transform_array + (uint64_t)0x30 * index);
		int transformIndex = Memory::ReadValue<int>(Global::pMemoryInterface, transformData.transform_indices + (uint64_t)0x4 * index);
		int pSafe = 0;
		while (transformIndex >= 0 && pSafe++ < 200) {
			matrix34_t matrix34 = Memory::ReadValue<matrix34_t>(Global::pMemoryInterface, transformData.transform_array + (uint64_t)0x30 * transformIndex);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix34.vec0));

			transformIndex = Memory::ReadValue<int>(Global::pMemoryInterface, transformData.transform_indices + (uint64_t)0x4 * transformIndex);
		}
	}

	return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

Player::Player(intptr_t address) :
	address(address), cachedEFTPlayerClassAddress(0),
	cachedTransformAddress(0)
{
}

ProfileInfo Player::GetProfileInfo() {
	if (!this->cachedProfileInfo.has_value()) {
		this->cachedProfileInfo = ProfileInfo{
			Memory::ReadChain<intptr_t>(
				Global::pMemoryInterface,
				this->address,
				{0x520, 0x28}
			)
		};
	}

	return this->cachedProfileInfo.value();
}

intptr_t Player::GetSkeletonTransformListValues() {
	if (!this->cachedEFTPlayerClassAddress) {
		this->cachedEFTPlayerClassAddress = Memory::ReadChain<intptr_t>(
			Global::pMemoryInterface,
			this->address,
			{ 0xA8, 0x28, 0x28, 0x10 }
		);
	}

	return this->cachedEFTPlayerClassAddress;
}


Vector3 Player::GetPosition() {
	if (!this->cachedTransformAddress) {
		this->cachedTransformAddress = Memory::ReadChain<intptr_t>(
			Global::pMemoryInterface,
			this->GetSkeletonTransformListValues(),
			{ 0x20, 0x10 }
		);
	}

	return this->cachedTransformAddress ? GetPositionFromTransform(this->cachedTransformAddress) : Vector3{ 0.f, 0.f, 0.f };
}

Vector3 Player::GetBone(EBone bone) {
	intptr_t boneAddress;
	if (this->cachedBones.contains(bone)) {
		boneAddress = this->cachedBones[bone];
	}
	else {
		boneAddress = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->GetSkeletonTransformListValues() + 0x20 + bone * 0x8) + 0x10);
		this->cachedBones[bone] = boneAddress;
	}

	return GetPositionFromTransform(boneAddress);
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
			Memory::ReadValue<intptr_t>(
				Global::pMemoryInterface,
				this->address + Offsets::Player::InventoryController
			)
		};
	}

	return this->cachedInventoryController.value();
}

Item Player::GetActiveWeapon() {
	return Item{ Memory::ReadChain<intptr_t>(Global::pMemoryInterface, this->address, {0x570, 0x60}) };
}
