#pragma once

#include <numbers>

struct BaseObject {
	uintptr_t previousObjectLink;
	uintptr_t nextObjectLink;
	uintptr_t object;
};