#pragma once

#include <numeric>
#include "ListNode.hpp"

class GameObjectManager {
public:
	GameObjectManager(intptr_t address) {
		this->address = address;
	}

	static GameObjectManager Get();

	ListNode GetActiveObjects() const;
	ListNode GetTaggedObjects() const;
	ListNode GetCameraList() const;

	intptr_t address;
};