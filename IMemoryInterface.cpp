#include "IMemoryInterface.h"
#include "Utils.h"

namespace Memory {
    wchar_t* Memory::ReadString(IMemoryInterface* pMemoryInterface, intptr_t address, bool sanitize) {
        if (!address || address < MINIMUM_ADDRESS_SIZE) {
            return nullptr;
        }

        int stringLength = Memory::ReadValue<int>(pMemoryInterface, address + 0x10);
        if (!stringLength) {
            return nullptr;
        }

        wchar_t* buffer = new wchar_t[stringLength + 1];
        if (!pMemoryInterface->ReadRaw(address + 0x14, buffer, sizeof(wchar_t) * stringLength)) {
            delete[] buffer;
            return nullptr;
        }

        if (sanitize) {
            for (int i = 0; i < stringLength; i++) {
                if (!Utils::IsValidWChar(buffer[i])) {
                    buffer[i] = L'?';
                }
            }
        }

        buffer[stringLength] = L'\0';

        return buffer;
    }
}