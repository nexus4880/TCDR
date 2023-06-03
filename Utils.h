#pragma once

#include <numbers>
#include <string>
#include "IMemoryInterface.h"

namespace Utils {
	unsigned long GetProcessIdByName(const wchar_t* ProcName);
	void TransposeMatrix(float* matrix);
	float LerpF(float a, float b, float t);
	const std::wstring GetRoleName(int role);
	bool IsValidWChar(wchar_t value);
	unsigned int HashString(const std::string& input);
	bool ContainsIgnoreCase(const std::wstring& str, const std::wstring& substr);
	std::wstring charToWstring(const char* input);
}