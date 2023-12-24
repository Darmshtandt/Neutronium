#pragma once

namespace Nt::Colors {
	constexpr Float4D Black			 = { 0.f,	0.f,	0.f,	1.f };
	constexpr Float4D White			 = { 1.f,	1.f,	1.f,	1.f };
	
	constexpr Float4D DarkGray		 = { 0.25f,	0.25f,	0.25f,	1.f };
	constexpr Float4D Gray			 = { 0.5f,	0.5f,	0.5f,	1.f };
	constexpr Float4D LightGray		 = { 0.75f,	0.75f,	0.75f,	1.f };
	
	constexpr Float4D DarkRed		 = { 0.5f,	0.f,	0.f,	1.f };
	constexpr Float4D Red			 = { 1.f,	0.f,	0.f,	1.f };
	constexpr Float4D LightRed		 = { 1.f,	0.5f,	0.5f,	1.f };
	
	constexpr Float4D DarkGreen		 = { 0.f,	0.5f,	0.f,	1.f };
	constexpr Float4D Green			 = { 0.f,	1.f,	0.f,	1.f };
	constexpr Float4D LightGreen	 = { 0.5f,	1.f,	0.5f,	1.f };
	
	constexpr Float4D DarkBlue		 = { 0.f,	0.f,	0.5f,	1.f };
	constexpr Float4D Blue			 = { 0.f,	0.f,	1.f,	1.f };
	constexpr Float4D LightBlue		 = { 0.5f,	0.5f,	1.f,	1.f };
	
	constexpr Float4D DarkYellow	 = { 0.5f,	0.5f,	0.f,	1.f };
	constexpr Float4D Yellow		 = { 1.f,	1.f,	0.f,	1.f };
	constexpr Float4D LightYellow	 = { 1.f,	1.f,	0.5f,	1.f };
	
	constexpr Float4D DarkTurquoise	 = { 0.f,	0.5f,	0.5f,	1.f };
	constexpr Float4D Turquoise		 = { 0.f,	1.f,	1.f,	1.f };
	constexpr Float4D LightTurquoise = { 0.5f,	1.f,	1.f,	1.f };
	
	constexpr Float4D DarkPurple	 = { 0.5f,	0.f,	0.5f,	1.f };
	constexpr Float4D Purple		 = { 1.f,	0.f,	1.f,	1.f };
	constexpr Float4D LightPurple	 = { 1.f,	0.5f,	1.f,	1.f };
	
	constexpr Float4D DarkOrange	 = { 0.75f,	0.25f,	0.f,	1.f };
	constexpr Float4D Orange		 = { 1.f,	0.5f,	0.f,	1.f };
	constexpr Float4D LightOrange	 = { 1.f,	0.75f,	0.25f,	1.f };

	__inline Byte4D ColorToByte4D(const Float4D& color) noexcept {
		return color * 255.f;
	}
}