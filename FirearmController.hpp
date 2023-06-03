#pragma once

#include "Item.hpp"
#include <numbers>

class FirearmController {
public:
	FirearmController(uint64_t address)
		: address(address)
	{
	}

	Item GetItem();
	bool IsAiming();

private:
	uint64_t address;
};