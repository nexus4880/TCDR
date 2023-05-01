#pragma once

namespace WinWrapper {
	short WGetAsyncKeyState(int key);
	void WSetForegroundWindow(void* handle);
}