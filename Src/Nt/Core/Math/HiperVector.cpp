// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Math/HiperVector.h>

namespace Nt {
	HiperVector::HiperVector(const uInt& dimension) : Position(dimension, 0)
	{
	}

	const Float & HiperVector::GetCoord(const uInt& axis) const {
		if (Position.size() <= axis)
			Raise("Out of range");

		return Position[axis];
	}
	Float& HiperVector::GetCoord(const uInt& axis) {
		if (Position.size() <= axis)
			Raise("Out of range");

		return Position[axis];
	}

	Float HiperVector::GetSquareDistance(const HiperVector& other) const {
		if (GetDimension() != other.GetDimension() || Position.empty())
			return 0.f;

		Float squareDistance = 0.f;
		for (uInt i = 0; i < Position.size(); ++i)
			squareDistance += powf(Position[i] - other.Position[i], 2.f);

		return squareDistance;
	}
	uInt HiperVector::GetDimension() const noexcept {
		return Position.size();
	}

	const Float& HiperVector::operator[](const uInt& axis) const {
		return GetCoord(axis);
	}
	Float& HiperVector::operator[](const uInt& axis) {
		return GetCoord(axis);
	}
}