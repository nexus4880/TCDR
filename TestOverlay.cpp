#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "TestOverlay.h"
#include <imgui.h>
#include <implot.h>
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
	ImPlot::CreateContext();
	bool hasError = false;
	Global::pSettings = std::make_unique<Settings>(Settings::FromFile("config.json", &hasError));
	if (hasError) {
		std::cout << "An error occurred while loading the config" << std::endl;
	}

	SetExitKey(KeyboardKey::KEY_NULL);
	if (std::filesystem::exists("items.json")) {
		std::wifstream fs{"items.json"};
		fs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
		nlohmann::json json_data{};
		fs >> json_data;
		for (auto& element : json_data.items()) {
			if (element.value().is_string()) {
				if (element.value().is_string()) {
					std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
					std::wstring key = conv.from_bytes(element.key());
					Global::itemTemplates[key] = element.value().get<std::wstring>();
				}
			}
		}
	}
	else {
		printf_s("Missing items.json?\n");
	}
}

TestOverlay::~TestOverlay() {
	ImPlot::DestroyContext();
}

void TestOverlay::OnFocusFound() {
	Global::pSettings = std::make_unique<Settings>(Settings::FromFile("config.json", nullptr));
}

void TestOverlay::OnFocusLost() {
	Global::pSettings->Serialize();
}

void TestOverlay::UpdateImGui() {
	Global::totalTime += GetFrameTime();
	Global::centerScreen = Vector2{GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};
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
				Hacks::DoKeybindActions();
				Hacks::DoNoRecoil();
			}
		}
		else {
			Global::gom = GameObjectManager::Get();
		}
	}
}

bool TestOverlay::ShouldShowMenu() {
	return this->isMenuOpen;
}

void TestOverlay::DrawImGui() {
	if (Global::pSettings->debug.enabled) {
		if (ImGui::Begin("Debug")) {
		}

		ImGui::End();
	}

	if (this->isMenuOpen) {
		if (ImGui::Selectable(TextFormat("GOM: %p", Global::gom.address))) {
			std::ostringstream stream{};
			stream << std::uppercase << std::hex << Global::gom.address;
			ImGui::SetClipboardText(stream.str().c_str());
		}

		if (ImGui::Selectable(TextFormat("ActiveCamera: %p", Global::activeCamera.address))) {
			std::ostringstream stream{};
			stream << std::uppercase << std::hex << Global::activeCamera.address;
			ImGui::SetClipboardText(stream.str().c_str());
		}

		if (ImGui::Selectable(TextFormat("GameWorld: %p", Global::gameWorld.address))) {
			std::ostringstream stream{};
			stream << std::uppercase << std::hex << Global::gameWorld.address;
			ImGui::SetClipboardText(stream.str().c_str());
		}

		if (Global::gameWorld.GetPlayers().size() > 0) {
			if (ImGui::Selectable(TextFormat("LocalPlayer: %p", Global::gameWorld.GetPlayers()[0].address))) {
				std::ostringstream stream{};
				stream << std::uppercase << std::hex << Global::gameWorld.GetPlayers()[0].address;
				ImGui::SetClipboardText(stream.str().c_str());
			}
		}

		ImGui::DragFloat("Polling Rate", &Global::pSettings->updateRate, 0.01f, 0.f, 5.f);
		ImGui::Checkbox("Show FPS", &Global::pSettings->showFPS);
		if (ImGui::CollapsingHeader("FOV")) {
			ImGui::Checkbox("Visualize", &Global::pSettings->visualizeImportantRadius);
			ImGui::DragFloat("Value", &Global::pSettings->importantRadius);
		}

		Hacks::DrawMenuOptions();
	}

	if (Global::pSettings->visualizeImportantRadius) {
		DrawCircleLines(
			static_cast<float>(Global::centerScreen.x),
			static_cast<float>(Global::centerScreen.y),
			Global::pSettings->importantRadius,
			MAGENTA
		);
	}

	if (Global::pSettings->showFPS) {
		DrawFPS(4, 4);
	}

	if (Global::gameWorld.address && Global::activeCamera.address) {
		Hacks::DrawLootESP();
		Hacks::DrawPlayerESP();
		Hacks::DrawSkeletonESP();
	}
}
