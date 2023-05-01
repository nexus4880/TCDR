#pragma once

#include <numbers>

struct BaseObject {
	uint64_t previousObjectLink;
	uint64_t nextObjectLink;
	uint64_t object;
};