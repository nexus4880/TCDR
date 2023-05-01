#pragma once

#include <numeric>

struct ListNode {
	intptr_t first;
	intptr_t last;

	intptr_t GetObject(const char* objectName);
};