// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Math/HiperVector.h>

namespace Nt {
	HiperVector::HiperVector(const uInt& dimension) : Position(dimension, 0)
	{
	}

	NT_FORCE_INLINE const Float& HiperVector::GetCoord(const uInt& axis) const {
		Assert(axis < Position.size(), "Out of range");
		return Position[axis];
	}
	NT_FORCE_INLINE Float& HiperVector::GetCoord(const uInt& axis) {
		Assert(axis < Position.size(), "Out of range");
		return Position[axis];
	}

	Float HiperVector::GetSquareDistance(const HiperVector& other) const {
		if (GetDimension() != other.GetDimension() || Position.empty())
			return 0.f;

		Float squareDistance = 0.f;
		for (uInt i = 0; i < Position.size(); ++i) {
			const Float different = Position[i] - other.Position[i];
			squareDistance += different * different;
		}

		return squareDistance;
	}
	NT_FORCE_INLINE uInt HiperVector::GetDimension() const noexcept {
		return Position.size();
	}

	NT_FORCE_INLINE const Float& HiperVector::operator [] (const uInt& axis) const {
		return GetCoord(axis);
	}
	NT_FORCE_INLINE Float& HiperVector::operator [] (const uInt& axis) {
		return GetCoord(axis);
	}
}