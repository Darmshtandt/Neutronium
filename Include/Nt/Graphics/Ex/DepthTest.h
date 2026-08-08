#pragma once
#include <Nt/Graphics/Renderer.h>

namespace NtEx {
	using namespace Nt;
	
	struct DepthTest {
		DepthMode Mode = DepthMode::LESS;
		Bool EnabledBuffer = true;
		Bool EnabledMask = true;
	};
}