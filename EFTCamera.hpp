#pragma once

#include <numbers>
#include <unordered_map>
#include "Matrix.hpp"
#include "Vector3.hpp"

class EFTCamera {
public:
	EFTCamera() : EFTCamera(0) {
	}

	EFTCamera(uint64_t address) {
		this->address = address;
	}

	Matrix GetMatrix() const;
	Vector3 WorldToScreen(Vector3 point) const;

	uint64_t address;
};