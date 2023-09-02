#include "ListNode.hpp"
#include "BaseObject.hpp"
#include "Global.hpp"

const int MAX_TRIES = 10000;

uintptr_t ListNode::GetObject(const char* objectName) {
	int count = 0;
	char name[256]{0};
	uintptr_t classNamePointer = 0x0;
	BaseObject activeObject = Memory::ReadValue<BaseObject>(Global::pMemoryInterface, this->last);
	BaseObject lastObject = Memory::ReadValue<BaseObject>(Global::pMemoryInterface, this->first);
	if (activeObject.object != 0x0) {
		while (activeObject.object != 0 && activeObject.object != lastObject.object) {
			count++;
			classNamePointer = Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, activeObject.object + 0x60);
			Global::pMemoryInterface->ReadRaw(classNamePointer + 0x0, &name, sizeof(name));
			if (!objectName || !strcmp(name, objectName)) {
				return activeObject.object;
			}

			if (count > MAX_TRIES) {
				return 0;
			}

			activeObject = Memory::ReadValue<BaseObject>(Global::pMemoryInterface, activeObject.nextObjectLink);
		}
	}

	if (lastObject.object != 0x0) {
		classNamePointer = Memory::ReadValue<uintptr_t>(Global::pMemoryInterface, lastObject.object + 0x60);
		Global::pMemoryInterface->ReadRaw(classNamePointer + 0x0, &name, 256);
		if (!objectName || !strcmp(name, objectName)) {
			return lastObject.object;
		}
	}

	return 0;
}
