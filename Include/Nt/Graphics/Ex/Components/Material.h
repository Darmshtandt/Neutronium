#pragma once

#include <Nt/Graphics/Ex/BaseComponent.h>
#include <Engine/DepthTest.h>
#include <Nt/Core/Colors.h>

namespace NtEx {
	using namespace Nt;

	struct Material : public BaseComponent {
		explicit Material(Object* pOwner) :
			BaseComponent(pOwner, Class<Material>::ID())
		{
		}

		DepthTest Depth = { };
		Float4D AmbientColor = Colors::White;
		Bool IsBillboard = false;
		Bool IsFullBright = false;
	};
}