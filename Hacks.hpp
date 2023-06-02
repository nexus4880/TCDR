#pragma once

#include <raylib.h>
#include <numbers>

namespace Hacks {
	extern bool isNoRecoilEnabled;
	void DoNoRecoil();
	void DrawMenuOptions();
	void DrawPlayerESP();
	void DrawSkeletonESP();
	void DrawLootESP();

	Vector3 ReadPosition(uint64_t transform);
}