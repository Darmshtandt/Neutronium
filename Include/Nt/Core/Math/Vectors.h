#pragma once

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>

#include <Nt/Core/Math/Vector2D.h>
#include <Nt/Core/Math/Vector3D.h>
#include <Nt/Core/Math/Vector4D.h>

namespace Nt {
	NT_API extern Float2D round(const Float2D Vec) noexcept;
	NT_API extern Float3D round(const Float3D Vec) noexcept;
	NT_API extern Float4D round(const Float4D Vec) noexcept;

	NT_API extern Double2D round(const Double2D Vec) noexcept;
	NT_API extern Double3D round(const Double3D Vec) noexcept;
	NT_API extern Double4D round(const Double4D Vec) noexcept;
}