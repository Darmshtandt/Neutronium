#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Graphics/Geometry/Shape.h>

namespace Nt::Primitive {
	NT_API _NODISCARD _CONSTEXPR20 Shape Quad(Float2D size, const Float4D& color, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });
	NT_API _NODISCARD _CONSTEXPR20 Shape Cube(Float3D size, const Float4D& color, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });
	NT_API _NODISCARD _CONSTEXPR20 Shape Pyramid(Float3D size, const Float4D& color, const FloatRect& textureRect = { 0.f, 0.f, 1.f, 1.f });
}