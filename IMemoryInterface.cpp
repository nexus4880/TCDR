#include "IMemoryInterface.h"
#include "Utils.h"

namespace Memory {
    const size_t readStringBufferSize = 255;
    wchar_t readStringBuffer[readStringBufferSize];

    std::wstring ReadString(IMemoryInterface* pMemoryInterface, uint64_t address, bool sanitize) {
        if (!address || address < MINIMUM_ADDRESS_SIZE) {
            return INVALID_ADDRESS_WSTRING;
        }

        int stringLength = Memory::ReadValue<int>(pMemoryInterface, address + 0x10);
        if (stringLength < 0 || stringLength > 255) {
            return INVALID_LENGTH_WSTRING;
        }

        if (stringLength == 0) {
            return L"";
        }

        if (!pMemoryInterface->ReadRaw(address + 0x14, readStringBuffer, sizeof(wchar_t) * stringLength)) {
            return NULL_WSTRING;
        }

        if (sanitize) {
            for (int i = 0; i < stringLength; i++) {
                if (!Utils::IsValidWChar(readStringBuffer[i])) {
                    readStringBuffer[i] = L'?';
                }
            }
        }

        readStringBuffer[stringLength] = L'\0';

        return std::wstring{readStringBuffer};
    }
}