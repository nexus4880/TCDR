#pragma once

#include <numbers>
#include <string>
#include "IMemoryInterface.h"

namespace Utils {
	unsigned long GetProcessIdByName(const wchar_t* ProcName);
	void TransposeMatrix(float* matrix);
	const wchar_t* WTextFormat(const char* text, ...);
	float LerpF(float a, float b, float t);
	std::string WCharToString(const wchar_t* text);
	const wchar_t* GetRoleName(int role);
	bool IsValidWChar(wchar_t value);
}