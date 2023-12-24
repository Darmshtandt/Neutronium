#include <Nt/Core/Math/Vectors.h>

namespace Nt {
	Float2D round(const Float2D& vec) noexcept {
		return Float2D(::roundf(vec.x), ::roundf(vec.y));
	}
	Float3D round(const Float3D& vec) noexcept {
		return Float3D(::roundf(vec.x), ::roundf(vec.y), ::roundf(vec.z));
	}
	Float4D round(const Float4D& vec) noexcept {
		return Float4D(::roundf(vec.x), ::roundf(vec.y), ::roundf(vec.z), ::roundf(vec.w));
	}

	Double2D round(const Double2D& vec) noexcept {
		return Double2D(::round(vec.x), ::round(vec.y));
	}
	Double3D round(const Double3D& vec) noexcept {
		return Double3D(::round(vec.x), ::round(vec.y), ::round(vec.z));
	}
	Double4D round(const Double4D& vec) noexcept {
		return Double4D(::round(vec.x), ::round(vec.y), ::round(vec.z), ::round(vec.w));
	}
}