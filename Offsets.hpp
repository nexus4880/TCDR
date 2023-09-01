#pragma once

#include <numbers>
#include <array>

namespace Offsets {
	extern uint64_t GOM;

	namespace GameObjectManager {
		extern uint64_t TaggedObjects;
		extern uint64_t ActiveObjects;
	}

	namespace GameWorld {
		extern uint64_t RegisteredPlayers;
	}

	namespace List {
		extern uint64_t Base;
		extern uint64_t Count;
		extern uint64_t FirstEntry;
	}

	namespace Player {
		extern uint64_t InventoryController;
	}

	namespace InventoryController {
		extern uint64_t InventoryClass;
		extern uint64_t IsInInventory;
	}

	namespace InventoryClass {
		extern uint64_t EquipmentClass;
	}
}