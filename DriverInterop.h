#pragma once

#include "IMemoryInterface.h"
#include <numeric>

class DriverInterop : public IMemoryInterface {
public:
    DriverInterop();
    DriverInterop(const DriverInterop& interop) = delete;
    ~DriverInterop();

    bool ReadRaw(uint64_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uint64_t address, void* pBuffer, unsigned long size);

    bool UpdateProcessId(const wchar_t* processName);
    uint64_t GetBaseAddress();
    uint64_t GetModuleBase();
    bool SetTargetModule(wchar_t* moduleName);

private:
    void* handle;
    unsigned long pid;
};