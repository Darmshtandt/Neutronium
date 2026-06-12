#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt::Colors {
	NT_CONSTEXPR Float4D Black(0.f, 0.f, 0.f, 1.f);
	NT_CONSTEXPR Float4D White(1.f, 1.f, 1.f, 1.f);

	NT_CONSTEXPR Float4D DarkGray(0.25f, 0.25f, 0.25f, 1.f);
	NT_CONSTEXPR Float4D Gray(0.5f, 0.5f, 0.5f, 1.f);
	NT_CONSTEXPR Float4D LightGray(0.75f, 0.75f, 0.75f, 1.f);

	NT_CONSTEXPR Float4D DarkRed(0.5f, 0.f, 0.f, 1.f);
	NT_CONSTEXPR Float4D Red(1.f, 0.f, 0.f, 1.f);
	NT_CONSTEXPR Float4D LightRed(1.f, 0.5f, 0.5f, 1.f);

	NT_CONSTEXPR Float4D DarkGreen(0.f, 0.5f, 0.f, 1.f);
	NT_CONSTEXPR Float4D Green(0.f, 1.f, 0.f, 1.f);
	NT_CONSTEXPR Float4D LightGreen(0.5f, 1.f, 0.5f, 1.f);

	NT_CONSTEXPR Float4D DarkBlue(0.f, 0.f, 0.5f, 1.f);
	NT_CONSTEXPR Float4D Blue(0.f, 0.f, 1.f, 1.f);
	NT_CONSTEXPR Float4D LightBlue(0.5f, 0.5f, 1.f, 1.f);

	NT_CONSTEXPR Float4D DarkYellow(0.5f, 0.5f, 0.f, 1.f);
	NT_CONSTEXPR Float4D Yellow(1.f, 1.f, 0.f, 1.f);
	NT_CONSTEXPR Float4D LightYellow(1.f, 1.f, 0.5f, 1.f);

	NT_CONSTEXPR Float4D DarkTurquoise(0.f, 0.5f, 0.5f, 1.f);
	NT_CONSTEXPR Float4D Turquoise(0.f, 1.f, 1.f, 1.f);
	NT_CONSTEXPR Float4D LightTurquoise(0.5f, 1.f, 1.f, 1.f);

	NT_CONSTEXPR Float4D DarkPurple(0.5f, 0.f, 0.5f, 1.f);
	NT_CONSTEXPR Float4D Purple(1.f, 0.f, 1.f, 1.f);
	NT_CONSTEXPR Float4D LightPurple(1.f, 0.5f, 1.f, 1.f);

	NT_CONSTEXPR Float4D DarkOrange(0.75f, 0.25f, 0.f, 1.f);
	NT_CONSTEXPR Float4D Orange(1.f, 0.5f, 0.f, 1.f);
	NT_CONSTEXPR Float4D LightOrange(1.f, 0.75f, 0.25f, 1.f);

	//NT_FORCE_INLINE NT_NODISCARD_CONSTEXPR Byte4D ColorToByte4D(const Float4D& color) noexcept {
	//	return Byte4D(color * 255.f);
	//}
	NT_FORCE_INLINE NT_NODISCARD_CONSTEXPR Float4D Byte4DToColor(const Byte4D& color) noexcept {
		return Float4D(color) / 255.f;
	}

	NT_FORCE_INLINE NT_NODISCARD Byte4D Color4DFromHex(std::string hex) {
		if (hex.front() == '#')
			hex.erase(hex.begin());

		if (hex.empty())
			return { };

		if (hex.size() < 6) {
			std::string correctHex = "000000FF";
			for (uInt i = 0; i < hex.size() * 2; ++i)
				correctHex[i] = hex[i / 2];

			hex = correctHex;
		}
		else if (hex.size() != 8) {
			hex = hex.substr(0, 6) + "FF";
		}

		const uLong value = std::stoul(hex, nullptr, 16);

		return {
			Byte((value >> 24) & 0xFF),
			Byte((value >> 16) & 0xFF),
			Byte((value >> 8) & 0xFF),
			Byte(value & 0xFF)
		};
	}
}