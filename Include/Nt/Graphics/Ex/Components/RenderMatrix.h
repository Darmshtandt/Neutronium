#pragma once

#include <Nt/Graphics/Ex/BaseComponent.h>
#include <Nt/Core/Math/Matrix4x4.h>

namespace NtEx {
	using namespace Nt;

	struct RenderMatrix : public BaseComponent {
		explicit RenderMatrix(Object* pOwner) :
			BaseComponent(pOwner, Class<RenderMatrix>::ID()) {
		}

		Matrix4x4 Local;
		Bool IsChanged = true;
	};
}