#pragma once

#include <numbers>
#include <array>

namespace Offsets {
	extern intptr_t GOM;

	namespace GameObjectManager {
		extern intptr_t TaggedObjects;
		extern intptr_t ActiveObjects;
	}

	namespace GameWorld {
		extern intptr_t RegisteredPlayers;
	}

	namespace List {
		extern intptr_t Base;
		extern intptr_t Count;
		extern intptr_t FirstEntry;
	}

	namespace Player {
		extern intptr_t InventoryController;
	}

	namespace InventoryController {
		extern intptr_t InventoryClass;
		extern intptr_t IsInInventory;
	}

	namespace InventoryClass {
		extern intptr_t EquipmentClass;
	}
}