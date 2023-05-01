#include "IMemoryInterface.h"

namespace Memory {
    wchar_t* Memory::ReadString(IMemoryInterface* pMemoryInterface, intptr_t address) {
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

        for (int i = 0; i < stringLength; i++) {
            if (!(buffer[i] < 0x0400 || (buffer[i] >= 0x0500 && buffer[i] < 0x4E00) || buffer[i] > 0x9FFF)) {
                buffer[i] = L'?';
            }
        }

        buffer[stringLength] = L'\0';

        return buffer;
    }
}