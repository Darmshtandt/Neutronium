#include <cmath>
#include <Nt/Core/Math/Vectors.h>

namespace Nt {
	Float2D round(const Float2D Vec) noexcept {
		return Float2D(::roundf(Vec.x), ::roundf(Vec.y));
	}
	Float3D round(const Float3D Vec) noexcept {
		return Float3D(::roundf(Vec.x), ::roundf(Vec.y), ::roundf(Vec.z));
	}
	Float4D round(const Float4D Vec) noexcept {
		return Float4D(::roundf(Vec.x), ::roundf(Vec.y), ::roundf(Vec.z), ::roundf(Vec.w));
	}

	Double2D round(const Double2D Vec) noexcept {
		return Double2D(::round(Vec.x), ::round(Vec.y));
	}
	Double3D round(const Double3D Vec) noexcept {
		return Double3D(::round(Vec.x), ::round(Vec.y), ::round(Vec.z));
	}
	Double4D round(const Double4D Vec) noexcept {
		return Double4D(::round(Vec.x), ::round(Vec.y), ::round(Vec.z), ::round(Vec.w));
	}
}