#pragma once

#include <numbers>
#include <optional>
#include <string>
#include "ProfileSettings.hpp"

enum EMemberCategory {
	Default = 0,
	Developer = 1,
	UniqueId = 2,
	Trader = 4,
	Group = 8,
	System = 16,
	ChatModerator = 32,
	ChatModeratorWithPermanentBan = 64,
	UnitTest = 128,
	Sherpa = 256,
	Emissary = 512
};

class ProfileInfo {
public:
	ProfileInfo(uint64_t address) {
		this->address = address;
	}

	unsigned char GetSide();
	int GetRegistrationDate();
	std::wstring GetNickname();
	std::wstring GetGroupID();
	std::wstring GetGroupHash();
	ProfileSettings GetSettings();
	bool IsPlayer();
	bool IsBoss();
	EMemberCategory GetMemberCategory();

	uint64_t address;

private:
	std::optional<unsigned char> cachedSide = std::nullopt;
	std::optional<int> cachedRegistrationDate = std::nullopt;
	std::optional<std::wstring> cachedNickname = std::nullopt;
	std::optional<std::wstring> cachedGroupID = std::nullopt;
	std::optional<std::wstring> cachedGroupHash = std::nullopt;
	std::optional<ProfileSettings> cachedProfileSettings = std::nullopt;
	std::optional<EMemberCategory> cachedMemberCategory = std::nullopt;
};