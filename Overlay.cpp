#include "Overlay.h"
#include "WinWrapper.h"
#include <iostream>

Overlay::Overlay(const std::string title, int updateRate) : title(title), isMenuOpen(false) {
}

Overlay::~Overlay() {
}

void Overlay::Update() {
}

void Overlay::Draw() {
}

bool Overlay::IsRunning() {
	return false;
}
