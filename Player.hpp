#pragma once

#include <numbers>
#include "ProfileInfo.hpp"
#include "InventoryController.hpp"
#include <raylib.h>
#include <array>
#include "Bones.hpp"
#include <map>
#include <optional>

#define BONE_PATH_SIZE 21

class Player {
public:
	Player(intptr_t address);

	ProfileInfo GetProfileInfo();
	intptr_t GetSkeletonTransformListValues();
	Vector3 GetPosition();
	Vector3 GetBone(EBone bone);
	void DrawBones(ProfileInfo& relativeInfo);
	Color GetColor(ProfileInfo inRelationTo);
	InventoryController GetInventoryController();
	Item GetActiveWeapon();

	intptr_t address;

private:
	std::map<EBone, intptr_t> cachedBones;
	std::optional<ProfileInfo> cachedProfileInfo = std::nullopt;
	intptr_t cachedEFTPlayerClassAddress;
	intptr_t cachedTransformAddress;
	std::optional<InventoryController> cachedInventoryController = std::nullopt;
	std::optional<Color> cachedColor = std::nullopt;

	inline static const std::array<std::array<EBone, 2>, BONE_PATH_SIZE> drawPaths
	{
			std::array<EBone, 2>{EBone::Pelvis, EBone::Spine1},
			std::array<EBone, 2>{EBone::Spine1, EBone::Spine2},
			std::array<EBone, 2>{EBone::Spine2, EBone::Spine3},
			std::array<EBone, 2>{EBone::Spine3, EBone::Neck},
			std::array<EBone, 2>{EBone::Neck, EBone::Head},
			std::array<EBone, 2>{EBone::Neck, EBone::RUpperArm},
			std::array<EBone, 2>{EBone::RUpperArm, EBone::RForearm1},
			std::array<EBone, 2>{EBone::RForearm1, EBone::RForearm3},
			std::array<EBone, 2>{EBone::Neck, EBone::LUpperArm},
			std::array<EBone, 2>{EBone::LUpperArm, EBone::LForearm1},
			std::array<EBone, 2>{EBone::LForearm1, EBone::LForearm3},
			std::array<EBone, 2>{EBone::Pelvis, EBone::LThigh1},
			std::array<EBone, 2>{EBone::Pelvis, EBone::RThigh1},
			std::array<EBone, 2>{EBone::RThigh1, EBone::RThigh2},
			std::array<EBone, 2>{EBone::RThigh2, EBone::RCalf},
			std::array<EBone, 2>{EBone::RCalf, EBone::RFoot},
			std::array<EBone, 2>{EBone::LThigh1, EBone::LThigh2},
			std::array<EBone, 2>{EBone::LThigh2, EBone::LCalf},
			std::array<EBone, 2>{EBone::LCalf, EBone::LFoot},
	};
}; 