#include "Global.hpp"

namespace Global {
#if !PRIMITIVES_ONLY
	IMemoryInterface* pMemoryInterface;
	std::unique_ptr<Settings> pSettings;
	GameObjectManager gom{0};
	GameWorld gameWorld{0};
	EFTCamera activeCamera{0};
	std::vector<std::wstring> lootESPFilters;
	std::map<std::wstring, std::wstring> itemTemplates;
	Vector2 centerScreen;
#endif

	float totalTime;
	float lastUpdated;
}