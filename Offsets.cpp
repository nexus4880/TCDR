#include "Offsets.hpp"

namespace Offsets {
	uintptr_t GOM = 0x017FFD28;

	namespace GameObjectManager {
		uintptr_t TaggedObjects = 0x8;
		uintptr_t ActiveObjects = 0x20;
	}

	namespace GameWorld {
		uintptr_t RegisteredPlayers = 0xC0;
	}

	namespace List {
		uintptr_t Base = 0x0010;
		uintptr_t Count = 0x0018;
		uintptr_t FirstEntry = 0x0020;
	}

	namespace Player {
		uintptr_t InventoryController = 0x570;
	}

	namespace InventoryController {
		uintptr_t InventoryClass = 0x130;
		uintptr_t IsInInventory = 0x178;
	}

	namespace InventoryClass {
		uintptr_t EquipmentClass = 0x10;
	}
}