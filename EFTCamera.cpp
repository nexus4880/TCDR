#include "EFTCamera.hpp"
#include "IMemoryInterface.h"
#include "Global.hpp"
#include "Utils.h"
#include "Matrix.hpp"
#include "Vector3.hpp"

Matrix EFTCamera::GetMatrix() const {
	return Memory::ReadChain<Matrix>(Global::pMemoryInterface, this->address, {0x30, 0x18, 0x0DC});
}

Vector3 EFTCamera::WorldToScreen(Vector3 point) const {
	/*Matrix matrix = this->GetMatrix();
	Utils::TransposeMatrix((float*)&matrix);

	Vector3 translationVector = Vector3{matrix.m3, matrix.m7, matrix.m11};
	Vector3 up = Vector3{matrix.m1, matrix.m5, matrix.m9};
	Vector3 right = Vector3{matrix.m0, matrix.m4, matrix.m8};

	Vector3 result{0.f, 0.f, 0.f};
	result.z = Vector3DotProduct(translationVector, point) + matrix.m15;
	if (result.z > 0.01f) {
		float y = Vector3DotProduct(point, up) + matrix.m13;
		float x = Vector3DotProduct(point, right) + matrix.m12;
		result.x = (GetMonitorWidth(0) / 2.f) * (1.f + x / result.z);
		result.y = (GetMonitorHeight(0) / 2.f) * (1.f - y / result.z);
	}

	return result;*/
	return {};
}
