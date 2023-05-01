#include "Global.hpp"

namespace Global {
	IMemoryInterface* pMemoryInterface;
	std::unique_ptr<Settings> pSettings;
	GameObjectManager gom{0};
	GameWorld gameWorld{0};
	EFTCamera activeCamera{0};
	unsigned int readCalls[60]{0};
	unsigned int writeCalls[60]{0};
	unsigned int currentFrame;
	float totalTime;
	float lastUpdated;
	unsigned int updateCount;
	std::vector<std::wstring> lootESPFilters;
	std::map<std::wstring, std::wstring> itemTemplates;
	Vector2 centerScreen;
}