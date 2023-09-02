#pragma once

#include "IMemoryInterface.h"

class DMAInterop : public IMemoryInterface {
public:
    DMAInterop();
    ~DMAInterop();

    bool UpdateProcessId(const wchar_t* processName);
    bool ReadRaw(uintptr_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uintptr_t address, void* pBuffer, unsigned long size);
    uintptr_t GetModuleBase();

private:
    uint32_t m_pid;
    void* m_handle;
};