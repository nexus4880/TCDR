#include "Offsets.hpp"

namespace Offsets {
	intptr_t GOM = 0x017FFD28;

	namespace GameObjectManager {
		intptr_t TaggedObjects = 0x8;
		intptr_t ActiveObjects = 0x20;
	}

	namespace GameWorld {
		intptr_t RegisteredPlayers = 0xA0;
	}

	namespace List {
		intptr_t Base = 0x0010;
		intptr_t Count = 0x0018;
		intptr_t FirstEntry = 0x0020;
	}

	namespace Player {
		intptr_t InventoryController = 0x568;
	}

	namespace InventoryController {
		intptr_t InventoryClass = 0x130;
		intptr_t IsInInventory = 0x178;
	}

	namespace InventoryClass {
		intptr_t EquipmentClass = 0x10;
	}
}