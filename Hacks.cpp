#include <imgui.h>
#include <imgui_internal.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "EFTCamera.hpp"
#include "Global.hpp"
#include "Hacks.hpp"
#include "Utils.h"
#include "WinWrapper.h"

const std::wstring DEFAULT_INVENTORY = L"Default Inventory";

namespace Hacks {
    const char* snapLineModes[4]{ "Off", "Center", "Bottom", "Top" };
    Vector4 minMaxValuesToWrite{ 0.f, 0.f, 0.f, 0.f };
    char lootESPFilterInputBuffer[255];

    Vector2 GetSnaplineBase() {
        switch (Global::pSettings->snapLines.activeMode) {
            case 1: {
                return Global::centerScreen;
            }
            case 2: {
                return Vector2{ GetRenderWidth() * 0.5f, static_cast<float>(GetRenderHeight()) };
            }
            case 3: {
                return Vector2{ GetRenderWidth() * 0.5f, 0.f };
            }
        }

        return Vector2{ 0.f, 0.f };
    }

    void DoNoRecoil() {
        if (Global::pSettings->noRecoil.enabled && Global::gameWorld.GetPlayers().size() > 0) {
            uint64_t pwa =
                Memory::ReadValue<uint64_t>(Global::pMemoryInterface, Global::gameWorld.GetPlayers()[0].address + 0x1A0);
            if (pwa) {
                uint64_t shootingg = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x48);
                if (shootingg) {
                    Memory::Write<float>(Global::pMemoryInterface, shootingg + 0x78,
                        Global::pSettings->noRecoil.shootinggIntensity);
                }

                uint64_t breath = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x28);
                if (breath) {
                    Memory::Write<float>(Global::pMemoryInterface, breath + 0xA4,
                        Global::pSettings->noRecoil.breathIntensity);
                }

                uint64_t motion = Memory::ReadValue<uint64_t>(Global::pMemoryInterface, pwa + 0x38);
                if (motion) {
                    Memory::Write<float>(Global::pMemoryInterface, motion + 0xD0,
                        Global::pSettings->noRecoil.motionIntensity);
                }
            }
        }
    }

    void DoKeybindActions() {
        if (WinWrapper::WGetAsyncKeyState(Global::pSettings->keybinds.lootItemFilterWhitelistMode) & 1) {
            Global::pSettings->lootESP.whitelist = !Global::pSettings->lootESP.whitelist;
        }
        
        if (WinWrapper::WGetAsyncKeyState(Global::pSettings->keybinds.addLootItemToFilters) & 1) {
            std::vector<WorldLootItem>& loot = Global::gameWorld.GetLoot();
            WorldLootItem* targetItem = nullptr;
            float targetDistance = FLT_MAX;
            for (WorldLootItem& item : loot) {
                Vector3 screenPosition = Global::activeCamera.WorldToScreen(item.GetPosition());
                if (screenPosition.z < 0.01f) {
                    continue;
                }

                float distance = Vector2Distance(Global::centerScreen, *(Vector2*)&screenPosition);
                if (distance < targetDistance) {
                    targetDistance = distance;
                    targetItem = &item;
                }
            }

            if (targetItem) {
                std::wstring name = targetItem->GetLocalizedName(nullptr);
                if (IS_VALID_WSTRING(name) && std::find(Global::pSettings->lootESP.filters.begin(), Global::pSettings->lootESP.filters.end(), name) == Global::pSettings->lootESP.filters.end()) {
                    Global::pSettings->lootESP.filters.push_back(targetItem->GetLocalizedName(nullptr));
                    Global::pSettings->Serialize();
                }
            }
        }
    }

    void DrawMenuOptions() {
        if (ImGui::CollapsingHeader("Recoil")) {
            ImGui::Indent();
            ImGui::Checkbox("Enabled", &Global::pSettings->noRecoil.enabled);
            ImGui::DragFloat("Shootingg Intensity", &Global::pSettings->noRecoil.shootinggIntensity, 0.01f, 0.f, 1.f);
            ImGui::DragFloat("Breath Intensity", &Global::pSettings->noRecoil.breathIntensity, 0.01f, 0.f, 1.f);
            ImGui::DragFloat("Motion Intensity", &Global::pSettings->noRecoil.motionIntensity, 0.01f, 0.f, 1.f);
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("ESP")) {
            ImGui::Indent();
            if (ImGui::CollapsingHeader("Snaplines")) {
                ImGui::Indent();
                ImGui::PushID("ESP_Snaplines");
                ImGui::Combo("Mode", &Global::pSettings->snapLines.activeMode, snapLineModes, 4);
                ImGui::Checkbox("Players", &Global::pSettings->snapLines.types[0]);
                ImGui::Checkbox("Bosses", &Global::pSettings->snapLines.types[1]);
                ImGui::Checkbox("AI", &Global::pSettings->snapLines.types[2]);
                ImGui::Unindent();
                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Box")) {
                ImGui::Indent();
                ImGui::PushID("ESP_Box");
                ImGui::Checkbox("Players", &Global::pSettings->boxESP.types[0]);
                ImGui::Checkbox("Bosses", &Global::pSettings->boxESP.types[1]);
                ImGui::Checkbox("AI", &Global::pSettings->boxESP.types[2]);
                ImGui::DragFloat("Factor", &Global::pSettings->boxESP.factor, 1.f, 1.f);
                ImGui::Unindent();
                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Skeleton")) {
                ImGui::Indent();
                ImGui::PushID("ESP_Skeleton");
                // ImGui::DragFloat("Close FOV", &Global::pSettings->skeletonESP.closeFOV);
                int* skeletonESPEntitiesCount = &Global::pSettings->skeletonESP.entities;
                if (ImGui::DragInt("Max Entities", skeletonESPEntitiesCount)) {
                    // We don't need to check to see if it is > the amount of people in the world
                    if (*skeletonESPEntitiesCount < 0) {
                        *skeletonESPEntitiesCount = 0;
                    }
                }

                ImGui::Checkbox("Players", &Global::pSettings->skeletonESP.types[0]);
                ImGui::Checkbox("Bosses", &Global::pSettings->skeletonESP.types[1]);
                ImGui::Checkbox("AI", &Global::pSettings->skeletonESP.types[2]);

                ImGui::DragFloat("Max Distance", &Global::pSettings->skeletonESP.distance);
                ImGui::Unindent();
                ImGui::PopID();
            }

            if (ImGui::CollapsingHeader("Loot")) {
                ImGui::Indent();
                ImGui::PushID("ESP_Loot");
                ImGui::Checkbox("Enabled", &Global::pSettings->lootESP.enabled);
                ImGui::Checkbox("Use Filter", &Global::pSettings->lootESP.useFilter);
                ImGui::Checkbox("Whitelist", &Global::pSettings->lootESP.whitelist);
                ImGui::DragFloat("Distance", &Global::pSettings->lootESP.distance);
                ImGui::InputText("Filter", lootESPFilterInputBuffer, 255);
                bool shouldClear = false;
                if (ImGui::Button("Add")) {
                    std::string tempStr{lootESPFilterInputBuffer};
                    Global::pSettings->lootESP.filters.push_back(std::wstring{tempStr.begin(), tempStr.end()});
                    shouldClear = true;
                }

                ImGui::SameLine();
                if (ImGui::Button("Clear")) {
                    shouldClear = true;
                }

                if (shouldClear) {
                    for (int i = 0; i < 255; i++) {
                        lootESPFilterInputBuffer[i] = '\0';
                    }
                }

                size_t filterCount = Global::pSettings->lootESP.filters.size();
                if (filterCount > 0) {
                    if (ImGui::CollapsingHeader("Active Filters")) {
                        for (size_t i = 0; i < filterCount; i++) {
                            if (ImGui::Selectable(TextFormat("%ws", Global::pSettings->lootESP.filters[i].c_str()))) {
                                Global::pSettings->lootESP.filters.erase(Global::pSettings->lootESP.filters.begin() + i);
                                break;
                            }
                        }
                    }
                }

                ImGui::Unindent();
                ImGui::PopID();
            }

            ImGui::Unindent();
        }
    }

    void DrawPlayerESP() {
        std::vector<Player>& players = Global::gameWorld.GetPlayers();
        size_t playerCount = players.size();
        if (playerCount < 2) {
            return;
        }

        Vector3 localPlayerPosition = players[0].GetPosition();
        ProfileInfo& localPlayerInfo = players[0].GetProfileInfo();
        for (size_t i = 1; i < playerCount; i++) {
            Player& player = players[i];
            ProfileInfo info = player.GetProfileInfo();
            Vector3 playerPosition = player.GetPosition();
            float distance = Vector3Distance(localPlayerPosition, playerPosition);
            Vector3 screenPosition = Global::activeCamera.WorldToScreen(playerPosition);
            if (screenPosition.x <= 0 || screenPosition.y <= 0 || screenPosition.x >= GetRenderWidth() ||
                screenPosition.y >= GetRenderHeight() || screenPosition.z <= 0.f) {
                continue;
            }

            bool printedName = false;
            std::wostringstream stream{};
            bool isInImportantRange =
                Vector2Distance(Global::centerScreen, *(Vector2*)&screenPosition) < Global::pSettings->importantRadius;
            unsigned char playerSide = info.GetSide();
            Color color = player.GetColor(localPlayerInfo);
            switch (playerSide) {
                case 1:
                case 2: {
                    if (isInImportantRange) {
                        stream << info.GetNickname();
                        printedName = true;
                    }

                    break;
                }
                case 4: {
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
            if (test > 1.f) test = 1.f;

            stream << std::format("[{:0.0f}m]", distance).c_str();
            std::wstring wideString = stream.str();
            std::string str = std::string(wideString.begin(), wideString.end());
            Vector2 textSize = MeasureTextEx(GetFontDefault(), str.data(), 1.f, 4.5f);
            float alphaDistanceMultiplier = Global::pSettings->skeletonESP.distance > distance ? test
                : isInImportantRange ? 0.375f
                : 0.15f;
            DrawText(str.data(), static_cast<int>(screenPosition.x - textSize.x * 0.5f),
                static_cast<int>(screenPosition.y - textSize.y * 0.5f), 1,
                ColorAlpha(player.GetColor(localPlayerInfo), alphaDistanceMultiplier));
            bool drawSnapline =
                Global::pSettings->snapLines.activeMode > 0 &&
                ((info.IsPlayer() && Global::pSettings->snapLines.types[0]) ||
                    (info.IsBoss() && Global::pSettings->snapLines.types[1]) ||
                    (!info.IsBoss() && Global::pSettings->snapLines.types[2]));
            if (drawSnapline) {
                if (!(Global::pSettings->skeletonESP.distance < distance) && !isInImportantRange) {
                    DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition,
                        ColorAlpha(player.GetColor(localPlayerInfo), test));
                }
                else if (isInImportantRange) {
                    DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition,
                        ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
                }
                else {
                    DrawLineV(GetSnaplineBase(), *(Vector2*)&screenPosition,
                        ColorAlpha(player.GetColor(localPlayerInfo), 0.1f));
                }
            }

            bool drawBoxESP = (info.IsPlayer() && Global::pSettings->boxESP.types[0]) ||
                (info.IsBoss() && Global::pSettings->boxESP.types[1]) || 
                (!info.IsBoss() && Global::pSettings->boxESP.types[2]);
            if (drawBoxESP) {
                Vector3 headPosition = Vector3Add(player.GetBone(EBone::Head), Vector3{ 0.f, 0.25f, 0.f});
                Vector3 headScreenPositionV3 = Global::activeCamera.WorldToScreen(headPosition);
                Vector2 headScreenPosition = *(Vector2*)&headScreenPositionV3;
                float scale = Global::pSettings->boxESP.factor / distance;
                Vector2 topLeft = Vector2Subtract(headScreenPosition, Vector2{ scale, 0.f });
                Vector2 bottomRight = Vector2Add(*(Vector2*)&screenPosition, Vector2{ scale, 0.f });
                DrawRectangleLines(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, color);
            };
        }
    }

    void DrawSkeletonESP() {
        if (Global::pSettings->skeletonESP.entities <= 0) {
            return;
        }

        std::vector<Player>& players = Global::gameWorld.GetPlayers();
        size_t playerCount = players.size();
        if (playerCount <= 1) {
            return;
        }

        std::vector<Player*> sortedPlayers{};
        Vector3 localPlayerPosition = players[0].GetPosition();
        ProfileInfo& localPlayerInfo = players[0].GetProfileInfo();
        for (size_t i = 1; i < playerCount; i++) {
            ProfileInfo& info = players[i].GetProfileInfo();
            bool shouldDraw =
                ((info.IsPlayer() && Global::pSettings->skeletonESP.types[0]) ||
                    (info.IsBoss() && Global::pSettings->skeletonESP.types[1]) ||
                    (!info.IsBoss() && Global::pSettings->skeletonESP.types[2]));
            if (!shouldDraw) {
                continue;
            }

            Vector3 worldPosition = players[i].GetPosition();
            float distance = Vector3Distance(localPlayerPosition, worldPosition);
            if (distance > Global::pSettings->skeletonESP.distance) {
                continue;
            }

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
            players[i].distance = distance;
            players[i].distance2d = fovDistance;

            sortedPlayers.push_back(&players[i]);
        }

        std::sort(sortedPlayers.begin(), sortedPlayers.end(),
            [](const Player* player1, const Player* player2) { return player1->distance2d < player2->distance2d; });

        int entityCount = Global::pSettings->skeletonESP.entities < sortedPlayers.size()
            ? Global::pSettings->skeletonESP.entities
            : sortedPlayers.size();
        float alpha = 1.f;
        for (int i = 0; i < entityCount; i++) {
            sortedPlayers[i]->DrawBones(alpha, localPlayerInfo);
            alpha -= 1.f / static_cast<float>(entityCount);
        }
    }

    void DrawLootESP() {
        if (!Global::pSettings->lootESP.enabled || Global::gameWorld.GetPlayers().size() <= 0) {
            return;
        }

        std::vector<WorldLootItem>& loot = Global::gameWorld.GetLoot();
        Vector3 localPlayerPosition = Global::gameWorld.GetPlayers()[0].GetPosition();
        for (size_t i = 0; i < loot.size(); i++) {
            Color color = VIOLET;
            Vector3 worldPosition = loot[i].GetPosition();
            Vector3 screenPosition = Global::activeCamera.WorldToScreen(worldPosition);
            if (screenPosition.z < 0.01f) {
                continue;
            }

            float distance = Vector3Distance(localPlayerPosition, loot[i].GetPosition());
            std::wstring itemName = loot[i].GetLocalizedName(nullptr);
            if (itemName == DEFAULT_INVENTORY) {
                itemName = L"DEAD";
                color = GRAY;
            }

            DrawText(std::format("{} [{:0.0f}m]", std::string{itemName.begin(), itemName.end()}, distance).c_str(),
                screenPosition.x, screenPosition.y, 1, color);
        }
    }

    struct transform_access_read_only_t {
        uint64_t transform_data{};
        uint64_t data;
        int index;
    };

    struct transform_data_t {
        uint64_t transform_array{};
        uint64_t transform_indices{};
    };

    struct matrix34_t {
        Vector4 vec0{};
        Vector4 vec1{};
        Vector4 vec2{};
    };

    Vector3 ReadPosition(uint64_t transform) {
        __m128 result{};

        const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
        const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
        const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

        transform_access_read_only_t pTransformAccessReadOnly =
            Memory::ReadValue<transform_access_read_only_t>(Global::pMemoryInterface, transform + 0x38);
        // pTransformAccessReadOnly.index
        unsigned int index = Memory::ReadValue<unsigned int>(Global::pMemoryInterface, transform + 0x40);
        transform_data_t transformData =
            Memory::ReadValue<transform_data_t>(Global::pMemoryInterface, pTransformAccessReadOnly.transform_data + 0x18);

        if (transformData.transform_array && transformData.transform_indices) {
            result =
                Memory::ReadValue<__m128>(Global::pMemoryInterface, transformData.transform_array + (uint64_t)0x30 * index);
            int transformIndex =
                Memory::ReadValue<int>(Global::pMemoryInterface, transformData.transform_indices + (uint64_t)0x4 * index);
            int pSafe = 0;
            while (transformIndex >= 0 && pSafe++ < 200) {
                matrix34_t matrix34 = Memory::ReadValue<matrix34_t>(
                    Global::pMemoryInterface, transformData.transform_array + (uint64_t)0x30 * transformIndex);

                __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));  // xxxx
                __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));  // yyyy
                __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));  // zwxy
                __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));  // wzyw
                __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));  // zzzz
                __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));  // yxwy
                __m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

                result = _mm_add_ps(
                    _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
                        _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
                        _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
                        _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
                            _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
                        _mm_add_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
                            _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
                            _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
                            tmp7)),
                    *(__m128*)(&matrix34.vec0));

                transformIndex = Memory::ReadValue<int>(Global::pMemoryInterface,
                    transformData.transform_indices + (uint64_t)0x4 * transformIndex);
            }
        }

        return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
    }
}
