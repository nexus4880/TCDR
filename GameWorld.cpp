#include "GameWorld.hpp"
#include "Offsets.hpp"
#include "Global.hpp"
#include "Utils.h"
#include <raymath.h>
#include "mdissect/mdissect.hpp"

GameWorld GameWorld::Get() {
	uintptr_t gameWorldAddressGameObject = Global::gom.GetActiveObjects().GetObject("GameWorld");
	if (!gameWorldAddressGameObject) {
		return GameWorld{0};
	}

	uintptr_t gameWorldAddress = Memory::ReadChain<uintptr_t>(
		Global::pMemoryInterface,
		gameWorldAddressGameObject,
		{0x30, 0x18, 0x28}
	);

	return GameWorld{gameWorldAddress};
}

std::vector<Player>& GameWorld::GetPlayers() {
	if (!this->cachedPlayers.has_value()) {
		std::vector<Player> players{};
		uintptr_t registeredPlayersAddress = Memory::ReadValue<uintptr_t>(
			Global::pMemoryInterface,
			this->address + Offsets::GameWorld::RegisteredPlayers
		);
		if (registeredPlayersAddress) {
			std::vector<uintptr_t> playerAddresses = Memory::ReadList<uintptr_t>(Global::pMemoryInterface, registeredPlayersAddress);
			for (int i = 0; i < playerAddresses.size(); i++) {
				players.push_back(Player{ playerAddresses[i]});
			}
		}

		this->cachedPlayers = players;
	}

	return this->cachedPlayers.value();
}

std::vector<WorldLootItem>& GameWorld::GetLoot() {
	size_t filtersCount = Global::pSettings->lootESP.filters.size();
	if (!this->cachedLoot.has_value()) {
		std::vector<uintptr_t> addresses = Memory::ReadList<uintptr_t>(Global::pMemoryInterface, Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, this->address + 0x80));
		size_t length = addresses.size();
		std::vector<WorldLootItem> loot{};
		loot.reserve(length);
		Vector3 localPlayerPosition = Global::gameWorld.GetPlayers()[0].GetPosition();
		for (size_t i = 0; i < length; i++) {
			std::string className = mdissect::mono_object{ addresses[i] }.vtable().mono_class().name();
			bool isCorpse = className == "ObservedCorpse" || className == "Corpse";
			if (className != "ObservedLootItem" && !isCorpse) {
				continue;
			}

			WorldLootItem lootItem{ addresses[i] };
			float distance = Vector3Distance(localPlayerPosition, lootItem.GetPosition());
			if (distance > Global::pSettings->lootESP.distance) {
				continue;
			}

			if (!isCorpse) {
				bool isLocalized = false;
				std::wstring itemName = lootItem.GetLocalizedName(&isLocalized);
				if (!isLocalized) {
					continue;
				}

				if (Global::pSettings->lootESP.useFilter && filtersCount > 0) {
					bool found = false;
					for (size_t i = 0; i < filtersCount; i++) {
						if (Utils::ContainsIgnoreCase(itemName, Global::pSettings->lootESP.filters[i])) {
							found = true;
							break;
						}
					}

					if (!found && Global::pSettings->lootESP.whitelist || found && !Global::pSettings->lootESP.whitelist) {
						continue;
					}
				}
			}
      
			loot.push_back(lootItem);
		}

		this->cachedLoot = loot;
	}
	
	return this->cachedLoot.value();
}
