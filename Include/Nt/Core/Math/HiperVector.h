#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	struct HiperVector {
		NT_API HiperVector(const uInt& dimension);

		template <uInt dimension>
		HiperVector(const Vector<Float, dimension>& point) :
			Position(dimension)
		{
			for (uInt i = 0; i < dimension; ++i)
				Position[i] = point.Array[i];
		}

		NT_API NT_NODISCARD const Float& GetCoord(const uInt& axis) const;
		NT_API NT_NODISCARD Float& GetCoord(const uInt& axis);

		NT_API NT_NODISCARD Float GetSquareDistance(const HiperVector& other) const;
		NT_API NT_NODISCARD uInt GetDimension() const noexcept;

		template <uInt dimension>
		NT_NODISCARD Vector<Float, dimension> GetVector() const {
			Vector<Float, dimension> vector;
			vector.Fill(0.f);

			for (uInt i = 0; i < std::min(Position.size(), dimension); ++i)
				vector.Array[i] = Position[i];

			return vector;
		}

		NT_API NT_NODISCARD const Float& operator [] (const uInt& axis) const ;
		NT_API NT_NODISCARD Float& operator [] (const uInt& axis);

		std::vector<Float> Position;
	};
}