#pragma once

#include <numbers>
#include <string>

class ItemTemplate {
public:
	ItemTemplate(intptr_t address) {
		this->address = address;
	}

	std::wstring GetId();
	std::wstring GetName();
	std::wstring GetShortName();
	bool IsQuestItem();

	intptr_t address;
};