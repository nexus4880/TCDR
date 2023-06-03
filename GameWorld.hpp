#pragma once

#include "IMemoryInterface.h"
#include "GameObjectManager.hpp"
#include "WorldLootItem.hpp"
#include <numbers>
#include <vector>
#include <optional>
#include "Player.hpp"

class GameWorld {
public:
	GameWorld(uint64_t address) {
		this->address = address;
	}

	static GameWorld Get();

	std::vector<Player>& GetPlayers();
	std::vector<WorldLootItem>& GetLoot();

	uint64_t address;

private:
	std::optional<std::vector<Player>> cachedPlayers = std::nullopt;
	std::optional<std::vector<WorldLootItem>> cachedLoot = std::nullopt;
};