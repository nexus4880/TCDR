#pragma once

#include "Overlay.h"

class TestOverlay : public Overlay
{
public:
	TestOverlay(const char* title, int updateRate);
	TestOverlay(const TestOverlay& overlay) = delete;
	~TestOverlay();
	void UpdateImGui() override;
	void DrawImGui() override;
	bool ShouldShowMenu() override;
	void OnFocusFound() override;
	void OnFocusLost() override;
};

