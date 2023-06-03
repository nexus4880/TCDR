#pragma once

#include <numeric>

struct ListNode {
	uint64_t first;
	uint64_t last;

	uint64_t GetObject(const char* objectName);
};