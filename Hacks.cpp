#include "Hacks.hpp"
#include <raylib.h>
#include "Global.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <raymath.h>
#include <string>
#include "EFTCamera.hpp"
#include "WinWrapper.h"
#include "Utils.h"
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <format>

namespace Hacks {
	const char* snapLineModes[4]{"Off", "Center", "Bottom", "Top"};
	Vector4 minMaxValuesToWrite{0.f, 0.f, 0.f, 0.f};

	Vector2 GetSnaplineBase() {
		switch (Global::pSettings->snapLines.activeMode) {
			case 1:
			{
				return Global::centerScreen;
			}
			case 2:
			{
				return Vector2{GetRenderWidth() * 0.5f, static_cast<float>(GetRenderHeight())};
			}
			case 3:
			{
				return Vector2{GetRenderWidth() * 0.5f, 0.f};
			}
		}

		return Vector2{0.f, 0.f};
	}

	void DoNoRecoil() {
		if (WinWrapper::WGetAsyncKeyState(Global::pSettings->keybinds.toggleNoRecoil) & 1) {
			Global::pSettings->noRecoil.enabled = !Global::pSettings->noRecoil.enabled;
		}

		if (Global::pSettings->noRecoil.enabled && Global::gameWorld.GetPlayers().size() > 0) {
			intptr_t pwa = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, Global::gameWorld.GetPlayers()[0].address + 0x1A0);
			if (pwa) {
				intptr_t shootingg = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, pwa + 0x48);
				if (shootingg) {
					Memory::Write<float>(Global::pMemoryInterface, shootingg + 0x78, Global::pSettings->noRecoil.shootinggIntensity);
				}

				intptr_t breath = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, pwa + 0x28);
				if (breath) {
					Memory::Write<float>(Global::pMemoryInterface, breath + 0xA4, Global::pSettings->noRecoil.breathIntensity);
				}

				intptr_t motion = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, pwa + 0x38);
				if (motion) {
					Memory::Write<float>(Global::pMemoryInterface, motion + 0xD0, Global::pSettings->noRecoil.motionIntensity);
				}
			}
		}
	}

	void DrawMenuOptions() {
		if (ImGui::CollapsingHeader("Recoil")) {
			ImGui::Checkbox("Enabled", &Global::pSettings->noRecoil.enabled);
			ImGui::DragFloat("Shootingg Intensity", &Global::pSettings->noRecoil.shootinggIntensity, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Breath Intensity", &Global::pSettings->noRecoil.breathIntensity, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Motion Intensity", &Global::pSettings->noRecoil.motionIntensity, 0.01f, 0.f, 1.f);
			if (ImGui::Button("Swap Nosway Values")) {
				intptr_t pwa = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, Global::gameWorld.GetPlayers()[0].address + 0x1A0);
				if (pwa) {
					intptr_t walk = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, pwa + 0x30);
					if (walk) {
						intptr_t minMax = Memory::ReadValue<intptr_t>(Global::pMemoryInterface, walk + 0x38);
						if (minMax) {
							Vector4 currentValues = Memory::ReadValue<Vector4>(Global::pMemoryInterface, minMax + 0x20);
							Memory::Write<Vector4>(Global::pMemoryInterface, minMax + 0x20, minMaxValuesToWrite);
							minMaxValuesToWrite = currentValues;
						}
					}
				}
			}
		}

		if (ImGui::CollapsingHeader("ESP")) {
			if (ImGui::CollapsingHeader("Snaplines")) {
				ImGui::Combo("Mode", &Global::pSettings->snapLines.activeMode, snapLineModes, 4);
				ImGui::Checkbox("Players", &Global::pSettings->snapLines.types[0]);
				ImGui::Checkbox("Bosses", &Global::pSettings->snapLines.types[1]);
				ImGui::Checkbox("AI", &Global::pSettings->snapLines.types[2]);
			}

			if (ImGui::CollapsingHeader("Skeleton")) {
				ImGui::DragFloat("Close FOV", &Global::pSettings->skeletonESP.closeFOV);
				ImGui::DragFloat("Far FOV", &Global::pSettings->skeletonESP.farFOV);
				ImGui::DragFloat("Max Distance", &Global::pSettings->skeletonESP.distance);
			}
		}
	}

	void DrawPlayerESP() {
		std::vector<Player>& players = Global::gameWorld.GetPlayers();
		int playerCount = players.size();
		if (playerCount < 2) {
			return;
		}

		Vector3 localPlayerPosition = players[0].GetPosition();
		ProfileInfo localPlayerInfo = players[0].GetProfileInfo();
		for (int i = 1; i < playerCount; i++) {
			Player& player = players[i];
			ProfileInfo info = player.GetProfileInfo();
			Vector3 playerPosition = player.GetPosition();
			float distance = Vector3Distance(localPlayerPosition, playerPosition);
			Vector3 screenPosition = Global::activeCamera.WorldToScreen(playerPosition);
			if (screenPosition.x <= 0 || screenPosition.y <= 0 || screenPosition.x >= GetRenderWidth() || screenPosition.y >= GetRenderHeight() || screenPosition.z <= 0.f) {
				continue;
			}

			std::wostringstream stream{};
			bool isInImportantRange = Vector2Distance(Global::centerScreen, *(Vector2*)&screenPosition) < Global::pSettings->importantRadius;
			unsigned char playerSide = info.GetSide();
			Color color = player.GetColor(localPlayerInfo);
			switch (playerSide) {
				case 1:
				case 2:
				{
					if (isInImportantRange) {
						stream << info.GetNickname() << L"\n";
					}

					break;
				}
				case 4:
				{
					if (info.GetSettings().GetRole() != 1 && !info.IsPlayer()) {
						stream << Utils::GetRoleName(info.GetSettings().GetRole()) << L"\n";
					}
				}
			}

			if (isInImportantRange) {
				ItemTemplate itemTemplate = player.GetActiveWeapon().GetTemplate();
				if (itemTemplate.address) {
					const wchar_t* templateId = itemTemplate.GetId();
					if (templateId) {
						if (Global::itemTemplates.contains(templateId)) {
							stream << Global::itemTemplates[templateId].c_str() << L"\n";
						}
						else {
							stream << L"unlocalized_weapon\n";
						}
					}
				}
				else {
					stream << L"no_template\n";
				}
			}


			stream << std::format("[{:0.0f}m]", distance).c_str();
			// Convert stream from wchar_t* to char* for raylib
			std::wstring str = stream.str();
			const size_t bufferSize = (str.size() + 1) * sizeof(char);
			char* buffer = new char[bufferSize];
			wcstombs_s(nullptr, buffer, bufferSize, str.c_str(), str.size());
			Vector2 textSize = MeasureTextEx(GetFontDefault(), buffer, 1.f, 4.5f);
			DrawText(
				buffer,
				static_cast<int>(screenPosition.x - textSize.x * 0.5f),
				static_cast<int>(screenPosition.y - textSize.y * 0.5f),
				1,
				color
			);

			delete[] buffer;
			if (Global::pSettings->snapLines.activeMode > 0) {
				if (info.IsPlayer()) {
					if (Global::pSettings->snapLines.types[0]) {
						DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, player.GetColor(localPlayerInfo));
					}
				}
				else if (info.IsBoss()) {
					if (Global::pSettings->snapLines.types[1]) {
						DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, player.GetColor(localPlayerInfo));
					}
				}
				else {
					if (Global::pSettings->snapLines.types[2]) {
						DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, player.GetColor(localPlayerInfo));
					}
				}
			}
		}
	}

	void DrawSkeletonESP() {
		std::vector<Player>& players = Global::gameWorld.GetPlayers();
		int playerCount = players.size();
		if (playerCount <= 1) {
			return;
		}

		Vector3 localPlayerPosition = players[0].GetPosition();
		ProfileInfo localPlayerInfo = players[0].GetProfileInfo();
		for (int i = 1; i < playerCount; i++) {
			Vector3 worldPosition = players[i].GetPosition();
			Vector3 screenPosition = Global::activeCamera.WorldToScreen(worldPosition);
			if (screenPosition.z < 0.01f) {
				continue;
			}

			float distance = Vector3Distance(localPlayerPosition, worldPosition);
			if (distance > Global::pSettings->skeletonESP.distance) {
				continue;
			}

			players[i].DrawBones(localPlayerInfo);
		}
	}
	
	void DrawLootESP() {
		std::vector<WorldLootItem> loot = Global::gameWorld.GetLoot();
		for (int i = 0; i < loot.size(); i++) {
			if (loot[i].IsContainer()) {
				continue;
			}

			Vector3 worldPosition = loot[i].GetPosition();
			Vector3 screenPosition = Global::activeCamera.WorldToScreen(worldPosition);
			if (screenPosition.z < 0.01f) {
				continue;
			}

			std::wstring itemName = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<intptr_t>(Global::pMemoryInterface, loot[i].address + 0x68));
			if (Global::itemTemplates.contains(itemName)) {
				itemName = Global::itemTemplates[itemName];
			}

			size_t length = itemName.size() + 1;
			char* buffer = new char[length];
			wcstombs_s(nullptr, buffer, length, itemName.c_str(), length);
			DrawText(buffer, screenPosition.x, screenPosition.y, 1, VIOLET);
			delete[] buffer;
		}
	}
}
