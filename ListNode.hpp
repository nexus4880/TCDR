#pragma once

#include <numeric>

struct ListNode {
	uintptr_t first;
	uintptr_t last;

	uintptr_t GetObject(const char* objectName);
};