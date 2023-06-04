#pragma once

#include <string>

class Overlay
{
public:
	Overlay(const std::string title, int updateRate);
	~Overlay();
	virtual void UpdateImGui() {
	}

	virtual void OnFocusFound() {
	}

	virtual void OnFocusLost() {
	}

	void Update();
	void Draw();
	bool IsRunning();
	virtual void DrawImGui() = 0;
	virtual bool ShouldShowMenu() = 0;

	bool isMenuOpen;
private:
	const std::string title;
};
