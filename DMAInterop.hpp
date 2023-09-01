#pragma once

#include "IMemoryInterface.h"

class DMAInterop : public IMemoryInterface {
public:
    DMAInterop();
    ~DMAInterop();

    bool UpdateProcessId(const wchar_t* processName);
    bool ReadRaw(uint64_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uint64_t address, void* pBuffer, unsigned long size);
    uint64_t GetModuleBase();

private:
    void* m_pid;
    void* m_handle;
};