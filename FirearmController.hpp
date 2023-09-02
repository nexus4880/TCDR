#pragma once

#include "Item.hpp"
#include <numbers>

class FirearmController {
public:
	FirearmController(uintptr_t address)
		: address(address)
	{
	}

	Item GetItem();
	bool IsAiming();

private:
	uintptr_t address;
};