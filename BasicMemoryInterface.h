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
    bool ReadRaw(uint64_t address, void* pBuffer, unsigned long size);
    bool WriteRaw(uint64_t address, void* pBuffer, unsigned long size);
    uint64_t GetModuleBase();
private:
    void* handle;
    unsigned long pid;
};

