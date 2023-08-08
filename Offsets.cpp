#include "Offsets.hpp"

namespace Offsets {
	uint64_t GOM = 0x017FFD28;

	namespace GameObjectManager {
		uint64_t TaggedObjects = 0x8;
		uint64_t ActiveObjects = 0x20;
	}

	namespace GameWorld {
		uint64_t RegisteredPlayers = 0xC0;
	}

	namespace List {
		uint64_t Base = 0x0010;
		uint64_t Count = 0x0018;
		uint64_t FirstEntry = 0x0020;
	}

	namespace Player {
		uint64_t InventoryController = 0x570;
	}

	namespace InventoryController {
		uint64_t InventoryClass = 0x130;
	}

	namespace InventoryClass {
		uint64_t EquipmentClass = 0x10;
	}
}