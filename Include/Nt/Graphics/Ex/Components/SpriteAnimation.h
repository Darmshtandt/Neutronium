#pragma once

#include <Nt/Graphics/Ex/BaseComponent.h>

namespace NtEx {
	using namespace Nt;

	struct SpriteAnimation : public BaseComponent {
		explicit SpriteAnimation(Object* pOwner) :
			BaseComponent(pOwner, Class<SpriteAnimation>::ID()) {
		}

		uInt FrameCount = 0;
		uInt FrameIndex = 0;
		Float TimeDelayMs = 0.f;
		Float DeltaTimeMs = 0.f;
		Bool IsStarted = false;
		Bool IsOnce = true;
	};
}