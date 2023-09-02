#pragma once

#include <numeric>
#include <vector>
#include <string>

const int MINIMUM_ADDRESS_SIZE = 2048;

const std::wstring NULL_WSTRING = L"_NULL_";
const std::wstring INVALID_ADDRESS_WSTRING = L"_INVALID_ADDRESS_";
const std::wstring INVALID_LENGTH_WSTRING = L"_INVALID_LENGTH_";

#define IS_VALID_WSTRING(str) \
    (str != NULL_WSTRING && str != INVALID_ADDRESS_WSTRING && str != INVALID_LENGTH_WSTRING)

__interface IMemoryInterface
{
public:
    bool UpdateProcessId(const wchar_t* processName);
    bool ReadRaw(uintptr_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uintptr_t address, void* pBuffer, unsigned long size);
    uintptr_t GetModuleBase();
};

namespace Memory {
    template <typename T>
    bool Read(IMemoryInterface* pMemoryInterface, uintptr_t address, T* data) {
        if (!address) {
            return false;
        }

        return pMemoryInterface->ReadRaw(address, data, sizeof(T));
    }

    template <typename T>
    T ReadValue(IMemoryInterface* pMemoryInterface, uintptr_t address) {
        T t{};
        if (!address) {
            return t;
        }

        pMemoryInterface->ReadRaw(address, &t, sizeof(T));
        return t;
    }

    template <typename T>
    T ReadChain(IMemoryInterface* pMemoryInterface, uintptr_t baseAddress, std::vector<uintptr_t> chain) {
        T result{};
        size_t chainSize = chain.size();
        if (chainSize <= 1) {
            return result;
        }

        for (size_t i = 0; i < chainSize; i++) {
            if (i != chainSize - 1) {
                baseAddress = Memory::ReadValue<uintptr_t>(pMemoryInterface, baseAddress + chain[i]);
            }
            else {
                pMemoryInterface->ReadRaw(baseAddress + chain[i], &result, sizeof(T));
            }
        }

        return result;
    }

    template <typename T>
    bool Write(IMemoryInterface* pMemoryInterface, uintptr_t address, const T& data) {
        return pMemoryInterface->WriteRaw(address, (void*)&data, sizeof(T));
    }

    template <typename T>
    std::vector<T> ReadList(IMemoryInterface* pMemoryInterface, uintptr_t address) {
        int length = Memory::ReadValue<int>(pMemoryInterface, address + 0x18);
        std::vector<T> result{};
        if (length <= 0 || length > 255) {
            return result;
        }

        result.reserve(length);
        T* buffer = new T[length];
        pMemoryInterface->ReadRaw(Memory::ReadValue<uintptr_t>(pMemoryInterface, address + 0x10) + 0x20, buffer, sizeof(T) * length);
        for (int i = 0; i < length; i++) {
            result.push_back(buffer[i]);
        }

        delete[] buffer;

        return result;
    }

    std::wstring ReadString(IMemoryInterface* pMemoryInterface, uintptr_t address, bool sanitize = true);
}