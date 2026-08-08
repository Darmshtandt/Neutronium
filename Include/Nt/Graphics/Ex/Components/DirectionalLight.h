#pragma once

#include <Nt/Graphics/Ex/BaseComponent.h>
#include <Nt/Core/Colors.h>

namespace NtEx {
	using namespace Nt;

	struct DirectionalLight : public BaseComponent {
		explicit DirectionalLight(Object* pOwner) :
			BaseComponent(pOwner, Class<DirectionalLight>::ID())
		{
		}

		Float3D Color = Colors::White;
		Float3D Direction = { 0.f, -1.f, 0.f };
		Bool Enabled = true;
	};
}