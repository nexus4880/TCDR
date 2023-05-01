#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include "Utils.h"
#include <utility>
#include <string>
#include <locale>
#include <codecvt>

namespace Utils {
    unsigned long GetProcessIdByName(const wchar_t* ProcName) {
        PROCESSENTRY32 pe32{sizeof(PROCESSENTRY32)};
        HANDLE hSnapshot = NULL;
        unsigned long result = 0;
        hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (wcscmp(ProcName, pe32.szExeFile) == 0) {
                    result = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }

        CloseHandle(hSnapshot);

        return result;
    }

    void TransposeMatrix(float* matrix) {
        std::swap(*(matrix + 1), *(matrix + 4));
        std::swap(*(matrix + 2), *(matrix + 8));
        std::swap(*(matrix + 3), *(matrix + 12));
        std::swap(*(matrix + 6), *(matrix + 9));
        std::swap(*(matrix + 7), *(matrix + 13));
        std::swap(*(matrix + 11), *(matrix + 14));
    }

    const wchar_t* WTextFormat(const char* text, ...) {
        static wchar_t* buffer;
        va_list args;
        va_start(args, text);
        std::vsnprintf(reinterpret_cast<char*>(buffer), sizeof(buffer), text, args);
        va_end(args);
        return buffer;
    }

    float LerpF(float a, float b, float t) {
        return a + t * (b - a);
    }

    std::string WCharToString(const wchar_t* text) {
        if (!text) {
            return "(null)";
        }

        int length = static_cast<int>(wcslen(text));
        int size = WideCharToMultiByte(CP_UTF8, 0, text, length, nullptr, 0, nullptr, nullptr);
        std::string result(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, text, length, result.data(), size, nullptr, nullptr);

        return result;
    }

    const wchar_t* GetRoleName(int role) {
        switch (role) {
            case 5:
            case 8:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 30:
            {
                return L"Goon";
            }
            case 0:
            {
                return L"Marksman";
            }
            case 1:
            {
                return L"Assault";
            }
            case 3:
            {
                return L"Reshala";
            }
            case 6:
            {
                return L"Killa";
            }
            case 7:
            {
                return L"P. Sherman";
            }
            case 9:
            {
                return L"Raider";
            }
            case 10:
            {
                return L"CAssault";
            }
            case 11:
            {
                return L"Gluhar";
            }
            case 17:
            {
                return L"Sanitar";
            }
            case 19:
            {
                return L"GAssault";
            }
            case 20:
            {
                return L"Sectant";
            }
            case 21:
            {
                return L"Priest";
            }
            case 22:
            {
                return L"Tagilla";
            }
            case 24:
            {
                return L"Rogue";
            }
            case 25:
            {
                return L"Santa";
            }
            case 26:
            {
                return L"Knight";
            }
            case 27:
            {
                return L"Big Pipe";
            }
            case 28:
            {
                return L"Bird Eye";
            }
            case 29:
            {
                return L"Sriracha";
            }
        }

        return L"Unknown";
    }
}