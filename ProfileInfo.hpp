#pragma once

#include <numbers>
#include <optional>
#include "ProfileSettings.hpp"

/*    [00][C] Default : System.Int32
    [00][C] Developer : System.Int32
    [00][C] UniqueId : System.Int32
    [00][C] Trader : System.Int32
    [00][C] Group : System.Int32
    [00][C] System : System.Int32
    [00][C] ChatModerator : System.Int32
    [00][C] ChatModeratorWithPermanentBan : System.Int32
    [00][C] UnitTest : System.Int32
    [00][C] Sherpa : System.Int32
    [00][C] Emissary : System.Int32*/

enum EMemberCategory {
	Default,
	Developer,
	UniqueId,
	Trader,
	Group,
	System,
	ChatModerator,
	ChatModeratorWithPermanentBan,
	UnitTest,
	Sherpa,
	Emissary
};

class ProfileInfo {
public:
	ProfileInfo(intptr_t address) {
		this->address = address;
	}

	unsigned char GetSide();
	int GetRegistrationDate();
	wchar_t* GetNickname();
	wchar_t* GetGroupID();
	ProfileSettings GetSettings();
	bool IsPlayer();
	bool IsBoss();
	EMemberCategory GetMemberCategory();

	intptr_t address;

private:
	std::optional<unsigned char> cachedSide = std::nullopt;
	std::optional<int> cachedRegistrationDate = std::nullopt;
	wchar_t* cachedNickname = nullptr;
	std::optional<wchar_t*> cachedGroupID = std::nullopt;
	std::optional<ProfileSettings> cachedProfileSettings = std::nullopt;
	std::optional<EMemberCategory> cachedMemberCategory = std::nullopt;
};