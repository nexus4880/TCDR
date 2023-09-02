#ifdef MEMORY_INTERFACE_DMA

#include "DMAInterop.hpp"
#include <stdexcept>
#include "vmmdll.h"

DMAInterop::DMAInterop() {
	m_pid = 0;
	char* args[3] = {"", "-device", "FPGA"};
	m_handle = VMMDLL_Initialize(3, args);
	if (!m_handle || m_handle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("VMMDLL_Initialize failed");
	}
}

DMAInterop::~DMAInterop() {
	VMMDLL_Close((VMM_HANDLE)m_handle);
}

bool DMAInterop::UpdateProcessId(const wchar_t* processName) {
	if (m_pid) {
		return true;
	}

	return VMMDLL_PidGetFromName((VMM_HANDLE)m_handle, "EscapeFromTarkov.exe", (PDWORD)&m_pid) != 0;
}

bool DMAInterop::ReadRaw(uintptr_t address, void* pBuffer, unsigned long size) {
	return VMMDLL_MemRead((VMM_HANDLE)m_handle, (DWORD)m_pid, address, static_cast<PBYTE>(pBuffer), size) != 0;
}

bool DMAInterop::WriteRaw(uintptr_t address, void* pBuffer, unsigned long size) {
	return VMMDLL_MemWrite((VMM_HANDLE)m_handle, (DWORD)m_pid, address, static_cast<PBYTE>(pBuffer), size) != 0;
}

uintptr_t DMAInterop::GetModuleBase() {
	return VMMDLL_ProcessGetModuleBase((VMM_HANDLE)m_handle, (DWORD)m_pid, L"UnityPlayer.dll");
}

#endif