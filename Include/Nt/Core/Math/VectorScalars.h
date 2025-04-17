#pragma once

#include <array>

namespace Nt {
	template <class _Ty> struct Vector2D;
	template <class _Ty> struct Vector3D;
	template <class _Ty> struct Vector4D;

	template <class _Ty, int size>
	struct VectorScalars {
		std::array<_Ty, size> Array = { };
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 2> {
		union {
			struct { _Ty x, y; };
			struct { _Ty r, g; };

			std::array<_Ty, 2> Array = { };
		};
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 3> {
		using Vec2 = Vector2D<_Ty>;

		union {
			struct { _Ty x, y, z; };
			struct { _Ty r, g, b; };

			struct { Vec2 xy; _Ty z; };
			struct { _Ty x; Vec2 yz; };

			struct { Vec2 rg; _Ty b; };
			struct { _Ty r; Vec2 gb; };

			std::array<_Ty, 3> Array = { };
		};
	};

	template <class _Ty>
	struct VectorScalars<_Ty, 4> {
		using Vec2 = Vector2D<_Ty>;
		using Vec3 = Vector3D<_Ty>;

		union {
			struct { _Ty x, y, z, w; };
			struct { _Ty r, g, b, z; };

			struct { Vec2 xy; _Ty z, w; };
			struct { _Ty x; Vec2 yz; _Ty w; };
			struct { _Ty x, y; Vec2 zw; };
			struct { Vec2 xy, zw; };

			struct { Vec2 rg; _Ty b, a; };
			struct { _Ty r; Vec2 gb; _Ty a; };
			struct { _Ty r, g; Vec2 ba; };
			struct { Vec2 rg, ba; };

			struct { Vec3 xyz; _Ty w; };
			struct { _Ty x; Vec3 yzw; };

			struct { Vec3 rgb; _Ty a; };
			struct { _Ty r; Vec3 gba; };

			std::array<_Ty, 4> Array = { };
		};
	};
}