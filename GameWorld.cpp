#include "GameWorld.hpp"
#include "Offsets.hpp"
#include "Global.hpp"

GameWorld GameWorld::Get() {
	uint64_t gameWorldAddressGameObject = Global::gom.GetActiveObjects().GetObject("GameWorld");
	if (!gameWorldAddressGameObject) {
		return GameWorld{0};
	}

	uint64_t gameWorldAddress = Memory::ReadChain<uint64_t>(
		Global::pMemoryInterface,
		gameWorldAddressGameObject,
		{0x30, 0x18, 0x28}
	);

	return GameWorld{gameWorldAddress};
}

std::vector<Player> GameWorld::GetPlayers() {
	if (!this->cachedPlayers.has_value()) {
		std::vector<Player> players{};
		uint64_t registeredPlayersAddress = Memory::ReadValue<uint64_t>(
			Global::pMemoryInterface,
			this->address + Offsets::GameWorld::RegisteredPlayers
		);
		if (registeredPlayersAddress) {
			std::vector<uint64_t> playerAddresses = Memory::ReadList<uint64_t>(Global::pMemoryInterface, registeredPlayersAddress);
			for (int i = 0; i < playerAddresses.size(); i++) {
				players.push_back(Player{ playerAddresses[i]});
			}
		}

		this->cachedPlayers = players;
	}

	return this->cachedPlayers.value();
}

std::vector<WorldLootItem> GameWorld::GetLoot() {
	if (!this->cachedLoot.has_value()) {
		std::vector<uint64_t> addresses = Memory::ReadList<uint64_t>(Global::pMemoryInterface, Memory::ReadValue<uint64_t>(Global::pMemoryInterface, this->address + 0x80));
		int length = addresses.size();
		std::vector<WorldLootItem> loot{};
		loot.reserve(length);
		for (int i = 0; i < length; i++) {
			loot.push_back(WorldLootItem{addresses[i]});
		}

		this->cachedLoot = loot;
	}
	
	return this->cachedLoot.value();
}
