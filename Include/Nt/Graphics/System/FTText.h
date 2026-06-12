#pragma once

#include <Nt/Graphics/Objects/IObject.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Geometry/Primitives.h>
#include <Nt/Graphics/System/FTFont.h>

namespace Nt {
	class FTText : public IObject {
	public:
		NT_API FTText(NotNull<FTFont*> pFont);
		NT_API FTText(NotNull<FTFont*> pFont, String text);
		NT_API ~FTText() noexcept override = default;

		NT_API void Render(NotNull<Renderer*> pRenderer) const override;
		NT_API void SetText(String text) noexcept;

		NT_API NT_NODISCARD Mesh& GetMesh() noexcept;

	private:
		Mesh m_Mesh = Primitive::Quad({ 1.f, 1.f }, Colors::White);
		String m_Text;
		FTFont* m_pFont;

	private:
		NT_API void _Initialize();
		NT_API void Render(NotNull<Renderer*> pRenderer, const uInt& offset, const uInt& verticesCount) const override;
	};
}