#ifdef MEMORY_INTERFACE_BASIC

#include "BasicMemoryInterface.h"
#include <Windows.h>
#include <Psapi.h>
#include "Utils.h"

#define PRIMITIVES_ONLY 1
#include "Global.hpp"

BasicMemoryInterface::~BasicMemoryInterface() {
    if (this->handle) {
        CloseHandle(this->handle);
    }
}

bool BasicMemoryInterface::UpdateProcessId(const wchar_t* processName) {
    if (this->pid && this->handle) {
        return true;
    }

    this->pid = Utils::GetProcessIdByName(processName);
    if (this->handle) {
        CloseHandle(this->handle);
        this->handle = nullptr;
    }

    if (this->pid) {
        this->handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->pid);
    }

    return this->pid && this->handle;
}

uintptr_t BasicMemoryInterface::GetModuleBase() {
    return 0x7FFCB7020000;
}

bool BasicMemoryInterface::ReadRaw(uintptr_t address, void* pBuffer, unsigned long size) {
    if (!this->pid || !this->handle || address < MINIMUM_ADDRESS_SIZE) {
        return false;
    }

    return ReadProcessMemory(this->handle, (LPCVOID)address, pBuffer, size, nullptr);
}

bool BasicMemoryInterface::WriteRaw(uintptr_t address, void* pBuffer, unsigned long size) {
    if (!this->pid || !this->handle || address < MINIMUM_ADDRESS_SIZE) {
        return false;
    }

    return WriteProcessMemory(this->handle, (LPVOID)address, pBuffer, size, nullptr);
}

#endif
