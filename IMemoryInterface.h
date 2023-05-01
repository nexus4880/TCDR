#pragma once

#include <numeric>
#include <vector>
#include <tuple>

#define MINIMUM_ADDRESS_SIZE 2048

__interface IMemoryInterface
{
public:
    intptr_t GetBaseAddress();
    bool UpdateProcessId(const wchar_t* processName);
    bool ReadRaw(intptr_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(intptr_t address, void* pBuffer, unsigned long size);
    intptr_t GetModuleBase();
    bool SetTargetModule(wchar_t* moduleName);
};

namespace Memory {
    template <typename T>
    bool Read(IMemoryInterface* pMemoryInterface, intptr_t address, T* data) {
        if (!address) {
            return false;
        }

        return pMemoryInterface->ReadRaw(address, data, sizeof(T));
    }

    template <typename T>
    T ReadValue(IMemoryInterface* pMemoryInterface, intptr_t address) {
        T t{};
        if (!address) {
            return t;
        }

        pMemoryInterface->ReadRaw(address, &t, sizeof(T));
        return t;
    }

    template <typename T>
    T ReadChain(IMemoryInterface* pMemoryInterface, intptr_t baseAddress, std::vector<intptr_t> chain) {
        T result{};
        size_t chainSize = chain.size();
        if (chainSize <= 1) {
            return result;
        }

        for (size_t i = 0; i < chainSize; i++) {
            if (i != chainSize - 1) {
                baseAddress = Memory::ReadValue<intptr_t>(pMemoryInterface, baseAddress + chain[i]);
            }
            else {
                pMemoryInterface->ReadRaw(baseAddress + chain[i], &result, sizeof(T));
            }
        }

        return result;
    }

    template <typename T>
    bool Write(IMemoryInterface* pMemoryInterface, intptr_t address, const T& data) {
        return pMemoryInterface->WriteRaw(address, (void*)&data, sizeof(T));
    }

    template <typename T>
    std::tuple<int, T*> ReadList(IMemoryInterface* pMemoryInterface, intptr_t address) {
        int length = Memory::ReadValue<int>(pMemoryInterface, address + 0x18);
        if (length == 0) {
            return std::tuple<int, T*>{0, nullptr};
        }

        T* buffer = new T[length];
        pMemoryInterface->ReadRaw(Memory::ReadValue<intptr_t>(pMemoryInterface, address + 0x10) + 0x20, buffer, sizeof(T) * length);

        return std::tuple<int, T*>{length, buffer};
    }

    wchar_t* ReadString(IMemoryInterface* pMemoryInterface, intptr_t address);
}