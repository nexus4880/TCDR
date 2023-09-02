#pragma once

#include <numbers>
#include <array>

namespace Offsets {
	extern uintptr_t GOM;

	namespace GameObjectManager {
		extern uintptr_t TaggedObjects;
		extern uintptr_t ActiveObjects;
	}

	namespace GameWorld {
		extern uintptr_t RegisteredPlayers;
	}

	namespace List {
		extern uintptr_t Base;
		extern uintptr_t Count;
		extern uintptr_t FirstEntry;
	}

	namespace Player {
		extern uintptr_t InventoryController;
	}

	namespace InventoryController {
		extern uintptr_t InventoryClass;
		extern uintptr_t IsInInventory;
	}

	namespace InventoryClass {
		extern uintptr_t EquipmentClass;
	}
}