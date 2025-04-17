#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct HiperVector {
		NT_API HiperVector(const uInt& dimension);

		template <uInt dimension>
		HiperVector(const Vector<Float, dimension>& point) :
			Position(point.Array.begin(), point.Array.end())
		{
		}

		NT_API _NODISCARD const Float& GetCoord(const uInt& axis) const;
		NT_API _NODISCARD Float& GetCoord(const uInt& axis);

		NT_API _NODISCARD Float GetSquareDistance(const HiperVector& other) const;
		NT_API _NODISCARD uInt GetDimension() const noexcept;

		template <uInt dimension>
		Vector<Float, dimension> GetVector() const {
			Vector<Float, dimension> vector;
			vector.Fill(0.f);

			for (uInt i = 0; i < std::min(Position.size(), dimension); ++i)
				vector.Array[i] = Position[i];

			return vector;
		}

		NT_API _NODISCARD const Float& operator [] (const uInt& axis) const ;
		NT_API _NODISCARD Float& operator [] (const uInt& axis);

		std::vector<Float> Position;
	};
}