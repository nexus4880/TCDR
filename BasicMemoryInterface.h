#pragma once

#include <numbers>
#include "IMemoryInterface.h"

class BasicMemoryInterface : public IMemoryInterface
{
public:
    BasicMemoryInterface() : handle(nullptr), pid(0) {
    }

    BasicMemoryInterface(const BasicMemoryInterface& basicMemoryInterface) = delete;
    ~BasicMemoryInterface();

    bool UpdateProcessId(const wchar_t* processName);
    uint64_t GetBaseAddress();
    bool ReadRaw(uint64_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uint64_t address, const void* pBuffer, unsigned long size);
    uint64_t GetModuleBase();
    bool SetTargetModule(wchar_t* moduleName);
private:
    void* handle;
    unsigned long pid;
};

