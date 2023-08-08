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
#include <xmmintrin.h>
#include <emmintrin.h>
#include "Vector4.hpp"

const std::wstring DEFAULT_INVENTORY_ID = L"55d7217a4bdc2d86028b456d";

namespace Hacks {
    const char* snapLineModes[4]{ "Off", "Center", "Bottom", "Top" };
    Vector4 minMaxValuesToWrite{ 0.f, 0.f, 0.f, 0.f };
    char lootESPFilterInputBuffer[255];

    Vector2 GetSnaplineBase() {
        /*switch (Global::pSettings->snapLines.activeMode) {
            case 1: {
                return Global::centerScreen;
            }
            case 2: {
                return Vector2{ GetRenderWidth() * 0.5f, static_cast<float>(GetRenderHeight()) };
            }
            case 3: {
                return Vector2{ GetRenderWidth() * 0.5f, 0.f };
            }
        }*/

        return Vector2{ 0.f, 0.f };
    }

    void DoNoRecoil() {
        /*if (Global::pSettings->noRecoil.enabled && Global::gameWorld.GetPlayers().size() > 0) {
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
        }*/
    }

    void DoKeybindActions() {
    }

    void DrawMenuOptions() {
    }

    void DrawPlayerESP() {
        /*std::vector<Player>& players = Global::gameWorld.GetPlayers();
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

                stream << player.GetFirearmController().GetItem().GetTemplate().GetLocalizedName().c_str() << L" ";
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
                    (!info.IsPlayer() && !info.IsBoss() && Global::pSettings->snapLines.types[2]));
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
                (!info.IsPlayer() && !info.IsBoss() && Global::pSettings->boxESP.types[2]);
            if (drawBoxESP) {
                Vector3 headPosition = Vector3Add(player.GetBone(EBone::Head), Vector3{ 0.f, 0.25f, 0.f});
                Vector3 headScreenPositionV3 = Global::activeCamera.WorldToScreen(headPosition);
                Vector2 headScreenPosition = *(Vector2*)&headScreenPositionV3;
                float scale = Global::pSettings->boxESP.factor / distance;
                Vector2 topLeft = Vector2Subtract(headScreenPosition, Vector2{ scale, 0.f });
                Vector2 bottomRight = Vector2Add(*(Vector2*)&screenPosition, Vector2{ scale, 0.f });
                DrawRectangleLines(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, color);
            };
        }*/
    }

    void DrawSkeletonESP() {
    }

    void DrawLootESP() {
        /*if (!Global::pSettings->lootESP.enabled || Global::gameWorld.GetPlayers().size() <= 0) {
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
            std::wstring itemName = NULL_WSTRING;
            if (loot[i].GetId() == DEFAULT_INVENTORY_ID) {
                itemName = L"DEAD";
                color = GRAY;
            }
            else {
                itemName = loot[i].GetLocalizedName(nullptr);
            }

            DrawText(std::format("{} [{:0.0f}m]", std::string{itemName.begin(), itemName.end()}, distance).c_str(),
                screenPosition.x, screenPosition.y, 1, color);
        }*/
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
