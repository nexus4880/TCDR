#pragma once

#include <numbers>
#include <raylib.h>
#include <unordered_map>

class EFTCamera {
public:
	EFTCamera() : EFTCamera(0) {
	}

	EFTCamera(uintptr_t address) {
		this->address = address;
	}

	Matrix GetMatrix() const;
	Vector3 WorldToScreen(Vector3 point) const;

	uintptr_t address;
};