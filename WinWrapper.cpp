#include "WinWrapper.h"
#include <Windows.h>

namespace WinWrapper {
    short WGetAsyncKeyState(int key) {
        return GetAsyncKeyState(key);
    }

    void WSetForegroundWindow(void* handle) {
        SetForegroundWindow((HWND)handle);
    }
}