#include "IMemoryInterface.h"
#include "Utils.h"

namespace Memory {
    std::wstring ReadString(IMemoryInterface* pMemoryInterface, uint64_t address, bool sanitize) {
        if (!address || address < MINIMUM_ADDRESS_SIZE) {
            return INVALID_ADDRESS_WSTRING;
        }

        int stringLength = Memory::ReadValue<int>(pMemoryInterface, address + 0x10);
        if (stringLength < 0) {
            return INVALID_LENGTH_WSTRING;
        }

        if (stringLength == 0) {
            return L"";
        }

        wchar_t* buffer = new wchar_t[stringLength + 1];
        if (!pMemoryInterface->ReadRaw(address + 0x14, buffer, sizeof(wchar_t) * stringLength)) {
            delete[] buffer;
            return NULL_WSTRING;
        }

        if (sanitize) {
            for (int i = 0; i < stringLength; i++) {
                if (!Utils::IsValidWChar(buffer[i])) {
                    buffer[i] = L'?';
                }
            }
        }

        buffer[stringLength] = L'\0';
        std::wstring result {buffer};
        delete[] buffer;

        return result;
    }
}