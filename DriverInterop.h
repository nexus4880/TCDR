#pragma once

#include "IMemoryInterface.h"
#include <numeric>

class DriverInterop : public IMemoryInterface {
public:
    DriverInterop();
    DriverInterop(const DriverInterop& interop) = delete;
    ~DriverInterop();

    bool UpdateProcessId(const wchar_t* processName);
    bool ReadRaw(uint64_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uint64_t address, void* pBuffer, unsigned long size);
    uint64_t GetModuleBase();

private:
    void* handle;
    unsigned long pid;
};