#include "GameWorld.hpp"
#include "Offsets.hpp"
#include "Global.hpp"

GameWorld GameWorld::Get() {
	intptr_t gameWorldAddressGameObject = Global::gom.GetActiveObjects().GetObject("GameWorld");
	if (!gameWorldAddressGameObject) {
		return GameWorld{0};
	}

	intptr_t gameWorldAddress = Memory::ReadChain<intptr_t>(
		Global::pMemoryInterface,
		gameWorldAddressGameObject,
		{0x30, 0x18, 0x28}
	);

	return GameWorld{gameWorldAddress};
}

std::vector<Player> GameWorld::GetPlayers() {
	if (!this->cachedPlayers.has_value()) {
		std::vector<Player> players{};
		intptr_t registeredPlayersAddress = Memory::ReadValue<intptr_t>(
			Global::pMemoryInterface,
			this->address + Offsets::GameWorld::RegisteredPlayers
		);
		if (registeredPlayersAddress) {
			std::tuple<int, intptr_t*> playerAddresses = Memory::ReadList<intptr_t>(Global::pMemoryInterface, registeredPlayersAddress);
			for (int i = 0; i < std::get<0>(playerAddresses); i++) {
				players.push_back(Player{std::get<1>(playerAddresses)[i]});
			}
		}

		this->cachedPlayers = players;
	}

	return this->cachedPlayers.value();
}

std::vector<WorldLootItem> GameWorld::GetLoot() {
	if (!this->cachedLoot.has_value()) {
		std::tuple<int, intptr_t*> list = Memory::ReadList<intptr_t>(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, this->address + 0x80));
		int count = std::get<0>(list);
		intptr_t* addresses = std::get<1>(list);
		std::vector<WorldLootItem> loot{};
		loot.reserve(count);
		for (int i = 0; i < count; i++) {
			loot.push_back(WorldLootItem{addresses[i]});
		}

		this->cachedLoot = loot;
	}
	
	return this->cachedLoot.value();
}