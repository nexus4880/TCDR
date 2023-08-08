#pragma once

#if !PRIMITIVES_ONLY
#include "IMemoryInterface.h"
#include "Settings.hpp"
#include "Player.hpp"
#include "GameObjectManager.hpp"
#include "GameWorld.hpp"
#include "EFTCamera.hpp"
#include <string>
#include <map>
#include <memory>
#endif
#include "Vector2.hpp"

namespace Global {
#if !PRIMITIVES_ONLY
	extern IMemoryInterface* pMemoryInterface;
	extern std::unique_ptr<Settings> pSettings;
	extern GameObjectManager gom;
	extern GameWorld gameWorld;
	extern EFTCamera activeCamera;
	extern std::map<std::wstring, std::wstring> itemTemplates;
	extern Vector2 centerScreen;
#endif

	extern float totalTime;
	extern float lastUpdated;
}