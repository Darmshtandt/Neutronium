#pragma once

#include <array>

namespace Nt {
	template <class _Ty> struct Vector2D;
	template <class _Ty> struct Vector3D;
	template <class _Ty> struct Vector4D;

	template <class _Ty, Int size>
	struct VectorScalars {
		_Ty Array[size];
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 2> {
		NT_CONSTEXPR VectorScalars() noexcept :
			x(0), y(0)
		{
		}

		union {
			struct { _Ty x, y; };
			struct { _Ty r, g; };

			_Ty Array[2];
		};
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 3> {
		using Vec2 = Vector2D<_Ty>;

		NT_CONSTEXPR VectorScalars() noexcept :
			x(0), y(0), z(0)
		{
		}

		union {
			struct { _Ty x, y, z; };
			struct { _Ty r, g, b; };

			struct { Vec2 xy; NT_MAYBE_UNUSED _Ty reserved_1; };
			struct { NT_MAYBE_UNUSED _Ty reserved_2; Vec2 yz; };

			struct { Vec2 rg; NT_MAYBE_UNUSED _Ty reserved_3; };
			struct { NT_MAYBE_UNUSED _Ty reserved_4; Vec2 gb; };

			_Ty Array[3];
		};
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 4> {
		using Vec2 = Vector2D<_Ty>;
		using Vec3 = Vector3D<_Ty>;

		NT_CONSTEXPR VectorScalars() noexcept :
			x(0), y(0), z(0), w(0)
		{
		}

		union {
			struct { _Ty x, y, z, w; };
			struct { _Ty r, g, b, a; };

			struct { Vec2 xy; NT_MAYBE_UNUSED _Ty reserved_1, reserved_2; };
			struct { NT_MAYBE_UNUSED _Ty reserved_3; Vec2 yz; NT_MAYBE_UNUSED _Ty reserved_4; };
			struct { NT_MAYBE_UNUSED _Ty reserved_5, reserved_6; Vec2 zw; };

			struct { Vec2 rg; NT_MAYBE_UNUSED _Ty reserved_7, reserved_8; };
			struct { NT_MAYBE_UNUSED _Ty reserved_9; Vec2 gb; NT_MAYBE_UNUSED _Ty reserved_10; };
			struct { NT_MAYBE_UNUSED _Ty reserved_11, reserved_12; Vec2 ba; };

			struct { Vec3 xyz; NT_MAYBE_UNUSED _Ty reserved_13; };
			struct { NT_MAYBE_UNUSED _Ty reserved_14; Vec3 yzw; };

			struct { Vec3 rgb; NT_MAYBE_UNUSED _Ty reserved_15; };
			struct { NT_MAYBE_UNUSED _Ty reserved_16; Vec3 gba; };

			_Ty Array[4];
		};
	};
}