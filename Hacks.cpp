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

#include <iostream>

namespace Hacks {
	const char* snapLineModes[4]{ "Off", "Center", "Bottom", "Top" };
	Vector4 minMaxValuesToWrite{ 0.f, 0.f, 0.f, 0.f };

	Vector2 GetSnaplineBase() {
		switch (Global::pSettings->snapLines.activeMode) {
			case 1:
			{
				return Global::centerScreen;
			}
			case 2:
			{
				return Vector2{ GetRenderWidth() * 0.5f, static_cast<float>(GetRenderHeight()) };
			}
			case 3:
			{
				return Vector2{ GetRenderWidth() * 0.5f, 0.f };
			}
		}

		return Vector2{ 0.f, 0.f };
	}

	void DoNoRecoil() {
		if (WinWrapper::WGetAsyncKeyState(Global::pSettings->keybinds.toggleNoRecoil) & 1) {
			Global::pSettings->noRecoil.enabled = !Global::pSettings->noRecoil.enabled;
		}

		if (Global::pSettings->noRecoil.enabled && Global::gameWorld.GetPlayers().size() > 0) {
			uint64_t pwa = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, Global::gameWorld.GetPlayers()[0].address + 0x1A0);
			if (pwa) {
				uint64_t shootingg = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x48);
				if (shootingg) {
					Memory::Write<float>(Global::pMemoryInterface, shootingg + 0x78, Global::pSettings->noRecoil.shootinggIntensity);
				}

				uint64_t breath = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x28);
				if (breath) {
					Memory::Write<float>(Global::pMemoryInterface, breath + 0xA4, Global::pSettings->noRecoil.breathIntensity);
				}

				uint64_t motion = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x38);
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
				uint64_t pwa = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, Global::gameWorld.GetPlayers()[0].address + 0x1A0);
				if (pwa) {
					uint64_t walk = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x30);
					if (walk) {
						uint64_t minMax = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, walk + 0x38);
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
				ImGui::PushID("ESP_Snaplines");
				ImGui::Combo("Mode", &Global::pSettings->snapLines.activeMode, snapLineModes, 4);
				ImGui::Checkbox("Players", &Global::pSettings->snapLines.types[0]);
				ImGui::Checkbox("Bosses", &Global::pSettings->snapLines.types[1]);
				ImGui::Checkbox("AI", &Global::pSettings->snapLines.types[2]);
				ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Box")) {
				ImGui::PushID("ESP_Box");
				ImGui::Checkbox("Players", &Global::pSettings->boxESP.types[0]);
				ImGui::Checkbox("Bosses", &Global::pSettings->boxESP.types[1]);
				ImGui::Checkbox("AI", &Global::pSettings->boxESP.types[2]);
				ImGui::DragFloat("Factor", &Global::pSettings->boxESP.factor, 1.f, 1.f);
				ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Skeleton")) {
				ImGui::PushID("ESP_Skeleton");
				//ImGui::DragFloat("Close FOV", &Global::pSettings->skeletonESP.closeFOV);
				int* skeletonESPEntitiesCount = &Global::pSettings->skeletonESP.entities;
				if (ImGui::DragInt("Max Entities", skeletonESPEntitiesCount, 1.f)) {
					// We don't need to check to see if it is > the amount of people in the world
					if (*skeletonESPEntitiesCount < 0) {
						*skeletonESPEntitiesCount = 0;
					}
				}

				ImGui::DragFloat("Max Distance", &Global::pSettings->skeletonESP.distance);
				ImGui::PopID();
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
		ProfileInfo& localPlayerInfo = players[0].GetProfileInfo();
		for (int i = 1; i < playerCount; i++) {
			Player& player = players[i];
			ProfileInfo info = player.GetProfileInfo();
			Vector3 playerPosition = player.GetPosition();
			float distance = Vector3Distance(localPlayerPosition, playerPosition);
			Vector3 screenPosition = Global::activeCamera.WorldToScreen(playerPosition);
			if (screenPosition.x <= 0 || screenPosition.y <= 0 || screenPosition.x >= GetRenderWidth() || screenPosition.y >= GetRenderHeight() || screenPosition.z <= 0.f) {
				continue;
			}

			bool printedName = false;
			std::wostringstream stream{};
			bool isInImportantRange = Vector2Distance(Global::centerScreen, *(Vector2*)&screenPosition) < Global::pSettings->importantRadius;
			unsigned char playerSide = info.GetSide();
			Color color = player.GetColor(localPlayerInfo);
			switch (playerSide) {
				case 1:
				case 2:
				{
					if (isInImportantRange) {
						stream << info.GetNickname();
						printedName = true;
					}

					break;
				}
				case 4:
				{
					if (info.GetSettings().GetRole() != 1 && !info.IsPlayer()) {
						stream << Utils::GetRoleName(info.GetSettings().GetRole());
						printedName = true;
					}
				}
			}

			if (isInImportantRange) {
				if (player.GetProfileInfo().IsPlayer()) {
					std::wstring id = info.GetGroupHash();
					if (id != NULL_WSTRING) {
						stream << L" [" << id << L"]";
					}
				}

				if (printedName) {
					stream << L"\n";
				}

				stream << player.GetActiveWeapon().GetTemplate().GetLocalizedName().c_str() << L" ";
			}

			float test = distance / Global::pSettings->skeletonESP.distance;
			test = -test + 1.1f;
			if (test > 1.f)
				test = 1.f;

			stream << std::format("[{:0.0f}m]", distance).c_str();
			std::wstring wideString = stream.str();
			std::string str = std::string(wideString.begin(), wideString.end());
			Vector2 textSize = MeasureTextEx(GetFontDefault(), str.data(), 1.f, 4.5f);
			float alphaDistanceMultiplier =
				Global::pSettings->skeletonESP.distance > distance ? test :
				isInImportantRange ? 0.375f : 0.15f;
			DrawText(str.data(), static_cast<int>(screenPosition.x - textSize.x * 0.5f), static_cast<int>(screenPosition.y - textSize.y * 0.5f), 1, ColorAlpha(player.GetColor(localPlayerInfo), alphaDistanceMultiplier));
			if (Global::pSettings->snapLines.activeMode > 0) {
				if (info.IsPlayer()) {
					if (Global::pSettings->snapLines.types[0]) {
						if (!(Global::pSettings->skeletonESP.distance < distance) && !isInImportantRange) {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), test));
						}
						else if (isInImportantRange) {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
						}
						else {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
						}
					}

					if (Global::pSettings->boxESP.types[0]) {
						Vector3 headPosition = player.GetBone(EBone::Head);
						Vector3 headScreenPositionV3 = Global::activeCamera.WorldToScreen(headPosition);
						Vector2 headScreenPosition = *(Vector2*)&headScreenPositionV3;
						float scale = Global::pSettings->boxESP.factor / distance;
						Vector2 topLeft = Vector2Subtract(headScreenPosition, Vector2{ scale, 0.f });
						Vector2 bottomRight = Vector2Add(*(Vector2*)&screenPosition, Vector2{ scale, 0.f });
						DrawRectangleLines(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, color);
					}
				}
				else if (info.IsBoss()) {
					if (Global::pSettings->snapLines.types[1]) {
						DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(ORANGE, 0.5f));
					}

					if (Global::pSettings->boxESP.types[1]) {
						Vector3 headPosition = player.GetBone(EBone::Head);
						Vector3 headScreenPositionV3 = Global::activeCamera.WorldToScreen(headPosition);
						Vector2 headScreenPosition = *(Vector2*)&headScreenPositionV3;
						float scale = Global::pSettings->boxESP.factor / distance;
						Vector2 topLeft = Vector2Subtract(headScreenPosition, Vector2{ scale, 0.f });
						Vector2 bottomRight = Vector2Add(*(Vector2*)&screenPosition, Vector2{ scale, 0.f });
						DrawRectangleLines(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, color);
					}
				}
				else {
					if (Global::pSettings->snapLines.types[2]) {
						if (!(Global::pSettings->skeletonESP.distance < distance) && !isInImportantRange) {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), test * 0.5f));
						}
						else if (isInImportantRange) {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
						}
						else {
							DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition, ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
						}
					}

					if (Global::pSettings->boxESP.types[2]) {
						Vector3 headPosition = player.GetBone(EBone::Head);
						Vector3 headScreenPositionV3 = Global::activeCamera.WorldToScreen(headPosition);
						Vector2 headScreenPosition = *(Vector2*)&headScreenPositionV3;
						float scale = Global::pSettings->boxESP.factor / distance;
						Vector2 topLeft = Vector2Subtract(headScreenPosition, Vector2{ scale, 0.f });
						Vector2 bottomRight = Vector2Add(*(Vector2*)&screenPosition, Vector2{ scale, 0.f });
						DrawRectangleLines(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, color);
					}
				}
			};
		}
	}

	void DrawSkeletonESP() {
		if (Global::pSettings->skeletonESP.entities <= 0) {
			return;
		}

		std::vector<Player>& players = Global::gameWorld.GetPlayers();
		std::vector<Player> sortedPlayers{};
		int playerCount = players.size();
		if (playerCount <= 1) {
			return;
		}

		Vector3 localPlayerPosition = players[0].GetPosition();
		ProfileInfo& localPlayerInfo = players[0].GetProfileInfo();
		for (int i = 1; i < playerCount; i++) {
			Vector3 worldPosition = players[i].GetPosition();
			Vector3 screenPosition = Global::activeCamera.WorldToScreen(worldPosition);
			if (screenPosition.z < 0.1f) {
				continue;
			}

			/*
				small issue with height if close to you and the center of vector2distance being the base of player
				i tried to move it up with screenpos.y+=3.f all the way to 30.f nothing changed helpppppp
			*/

			float fovDistance = Vector2Distance(Global::centerScreen, *(Vector2*)&screenPosition);
			players[i].isInImportantRange = fovDistance < Global::pSettings->importantRadius;
			float distance = Vector3Distance(localPlayerPosition, worldPosition);
			players[i].distance = distance;
			players[i].distance2d = fovDistance;
			if (distance > Global::pSettings->skeletonESP.distance) {
				continue;
			}

			sortedPlayers.push_back(players[i]);
		}

		std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& player1, const Player& player2) {
			return player1.distance2d < player2.distance2d;
			});

		int entityCount =
			Global::pSettings->skeletonESP.entities < sortedPlayers.size() ?
			Global::pSettings->skeletonESP.entities : sortedPlayers.size();
		float alpha = 1.f;
		for (int i = 0; i < entityCount; i++) {
			sortedPlayers[i].DrawBones(alpha, localPlayerInfo);
			alpha -= 1.f / static_cast<float>(entityCount);
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

			std::wstring itemName = Memory::ReadString(Global::pMemoryInterface, Memory::ReadValue<uint64_t>(Global::pMemoryInterface, loot[i].address + 0x68));
			if (Global::itemTemplates.contains(itemName)) {
				itemName = Global::itemTemplates[itemName];
			}

			std::string drawText(itemName.begin(), itemName.end());
			DrawText(drawText.data(), screenPosition.x, screenPosition.y, 1, VIOLET);
		}
	}
}
