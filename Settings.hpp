#pragma once

#include <filesystem>

struct Settings {
	static Settings FromFile(std::filesystem::path file, bool* hasErrorOccurred);
	void Serialize();

	struct NoRecoil {
		bool enabled = false;
		float shootinggIntensity = 1.f;
		float breathIntensity = 1.f;
		float motionIntensity = 1.f;
	};
	
	struct SnapLines {
		int activeMode = 0;
		//			  player boss   AI
		bool types[3]{false, false, false};
	};

	struct SkeletonESP {
		float distance = 100.f;
		float closeFOV = 100.f;
		int entities = 10;
	};

	struct Keybinds {
		int toggleNoRecoil = 0x77;
	};

	struct Debug {
		bool enabled = false;
	};

	bool showFPS = true;
	bool visualizeImportantRadius = true;
	float importantRadius = 290.f;
	float updateRate = 1.f;
	NoRecoil noRecoil;
	SnapLines snapLines;
	SkeletonESP skeletonESP;
	Keybinds keybinds;
	Debug debug;
};