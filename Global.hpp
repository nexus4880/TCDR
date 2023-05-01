#pragma once

#ifndef PRIMITIVES_ONLY
#include "IMemoryInterface.h"
#include "Settings.hpp"
#include "Player.hpp"
#include "GameObjectManager.hpp"
#include "GameWorld.hpp"
#include "EFTCamera.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#endif

namespace Global {
#ifndef PRIMITIVES_ONLY
	extern IMemoryInterface* pMemoryInterface;
	extern std::unique_ptr<Settings> pSettings;
	extern GameObjectManager gom;
	extern GameWorld gameWorld;
	extern EFTCamera activeCamera;
	extern std::vector<std::wstring> lootESPFilters;
	extern std::map<std::wstring, std::wstring> itemTemplates;
	extern Vector2 centerScreen;
#endif
	extern unsigned int readCalls[60];
	extern unsigned int writeCalls[60];
	extern unsigned int currentFrame;
	extern float totalTime;
	extern float lastUpdated;
	extern unsigned int updateCount;
}