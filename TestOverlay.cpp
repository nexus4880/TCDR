#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "TestOverlay.h"
#include "BaseObject.hpp"
#include "GameWorld.hpp"
#include "Offsets.hpp"
#include "Player.hpp"
#include <vector>
#include <iostream>
#include "Global.hpp"
#include "Hacks.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Utils.h"
#include <locale>
#include <codecvt>
#include "mdissect/mdissect.hpp"

namespace nlohmann {
	template <>
	struct adl_serializer<std::wstring> {
		static void to_json(json& j, const std::wstring& value) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			j = conv.to_bytes(value);
		}

		static void from_json(const json& j, std::wstring& value) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			value = conv.from_bytes(j.get<std::string>());
		}
	};

	template <typename _CharT, typename _Traits>
	std::basic_istream<_CharT, _Traits>& operator>>(std::basic_istream<_CharT, _Traits>& stream, json& j) {
		std::wstringstream ws;
		ws << stream.rdbuf();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string utf8_str = conv.to_bytes(ws.str());
		j = json::parse(utf8_str);

		return stream;
	}
}


TestOverlay::TestOverlay(const char* title, int updateRate) :
	Overlay(title, updateRate)
{
	bool hasError = false;
	Global::pSettings = std::make_unique<Settings>(Settings::FromFile("config.json", &hasError));
	if (hasError) {
		std::cout << "An error occurred while loading the config" << std::endl;
	}

	if (std::filesystem::exists("items.json")) {
		std::wifstream fs{"items.json"};
		fs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
		nlohmann::json json_data{};
		fs >> json_data;
		for (auto& element : json_data.items()) {
			std::string keyStr = element.key();
			std::wstring key{keyStr.begin(), keyStr.end()};
			Global::itemTemplates[key] = element.value().get<std::wstring>();
		}
	}
	else {
		printf_s("Missing items.json?\n");
	}

	mdissect::read_memory = [](uint64_t address, void* buffer, size_t size) -> bool {
		return Global::pMemoryInterface->ReadRaw(address, buffer, size);
	};

	mdissect::write_memory = [](uint64_t address, const void* buffer, size_t size) -> bool {
		return Global::pMemoryInterface->WriteRaw(address, buffer, size);
	};
}

TestOverlay::~TestOverlay() {
}

void TestOverlay::OnFocusFound() {
	Global::pSettings = std::make_unique<Settings>(Settings::FromFile("config.json", nullptr));
}

void TestOverlay::OnFocusLost() {
	Global::pSettings->Serialize();
}

void TestOverlay::UpdateImGui() {
	/*Global::totalTime += GetFrameTime();
	Global::centerScreen = Vector2{GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};
	Hacks::DoKeybindActions();
	if (Global::pMemoryInterface->UpdateProcessId(L"EscapeFromTarkov.exe")) {
		if (Global::gom.address) {
			if (Global::totalTime >= Global::lastUpdated + Global::pSettings->updateRate) {
				Global::lastUpdated = Global::totalTime;
				Global::activeCamera.address = 0;
				Global::gameWorld = GameWorld::Get();
				if (Global::gameWorld.address) {
					Global::activeCamera = EFTCamera{Global::gom.GetCameraList().GetObject(nullptr)};
				}
			}

			if (Global::gameWorld.address && Global::activeCamera.address) {
				Hacks::DoNoRecoil();
			}
		}
		else {
			Global::gom = GameObjectManager::Get();
		}
	}*/
}

bool TestOverlay::ShouldShowMenu() {
	return this->isMenuOpen;
}

void TestOverlay::DrawImGui() {
}
