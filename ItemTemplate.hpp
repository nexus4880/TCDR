#pragma once

#include <numbers>

class ItemTemplate {
public:
	ItemTemplate(intptr_t address) {
		this->address = address;
	}

	wchar_t* GetId();
	wchar_t* GetName();
	wchar_t* GetShortName();
	bool IsQuestItem();

	intptr_t address;
};