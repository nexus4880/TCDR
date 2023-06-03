#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include "Utils.h"
#include <utility>
#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>

namespace Utils {
    unsigned long GetProcessIdByName(const wchar_t* ProcName) {
        PROCESSENTRY32 pe32{ sizeof(PROCESSENTRY32) };
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

    float LerpF(float a, float b, float t) {
        return a + t * (b - a);
    }

    const std::wstring GetRoleName(int role) {
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
            return L"Minion";
        }
        case 0:
        {
            return L"Sniper";
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
            return L"Cultist";
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

    bool IsValidWChar(wchar_t value) {
        return
            value >= 48 && value <= 57 || // 0-9
            value >= 65 && value <= 90 || // A-Z
            value >= 97 && value <= 122 ||  // a-z
            value == L'-' || value == L'_'; // only special characters I know of that can be used
    }

    unsigned int HashString(const std::string& input) {
        const unsigned int FNV_PRIME = 16777619;
        const unsigned int OFFSET_BASIS = 2166136261;
        unsigned int hash = OFFSET_BASIS;
        for (char c : input) {
            hash ^= static_cast<unsigned int>(c);
            hash *= FNV_PRIME;
        }

        return hash;
    }

    bool ContainsIgnoreCase(const std::wstring& str, const std::wstring& substr) {
        std::wstring strLower = str;
        std::wstring substrLower = substr;

        std::transform(strLower.begin(), strLower.end(), strLower.begin(), std::towlower);
        std::transform(substrLower.begin(), substrLower.end(), substrLower.begin(), std::towlower);

        return strLower.find(substrLower) != std::wstring::npos;
    }

    std::wstring charToWstring(const char* input)
    {
        int length = MultiByteToWideChar(CP_UTF8, 0, input, -1, nullptr, 0);
        std::wstring wideString(length, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, input, -1, &wideString[0], length);
        return wideString;
    }
}
