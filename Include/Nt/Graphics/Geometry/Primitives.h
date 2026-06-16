#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Graphics/Geometry/Shape.h>
#include <Nt/Core/Colors.h>

namespace Nt::Primitive {
	NT_NODISCARD_CONSTEXPR NT_API 
	Shape Quad(Float2D size, const Float4D& color = Colors::White, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });

	NT_NODISCARD_CONSTEXPR NT_API 
	Shape Cube(Float3D size, const Float4D& color = Colors::White, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });

	NT_NODISCARD_CONSTEXPR NT_API 
	Shape Pyramid(Float3D size, const Float4D& color = Colors::White, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });

	NT_NODISCARD_CONSTEXPR NT_API 
	Shape Line(Float length, const Float4D& color = Colors::White);
}