#pragma once

#include <numbers>
#include <raylib.h>
#include <unordered_map>
#include <optional>

class EFTCamera {
public:
	EFTCamera() : EFTCamera(0) {
	}

	EFTCamera(uint64_t address) {
		this->address = address;
	}

	Matrix GetMatrix();
	Vector3 WorldToScreen(Vector3 point);

	uint64_t address;
	std::optional<Matrix> cachedMatrix = std::nullopt;
};