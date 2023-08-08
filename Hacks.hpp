#pragma once

#include <numbers>

namespace Hacks {
	extern bool isNoRecoilEnabled;
	void DoNoRecoil();
	void DoKeybindActions();
	void DrawMenuOptions();
	void DrawPlayerESP();
	void DrawSkeletonESP();
	void DrawLootESP();

	Vector3 ReadPosition(uint64_t transform);
}