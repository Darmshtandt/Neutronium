#include <GL/GLEW.h>
#include <Nt/Graphics/System/FTText.h>
#include <Nt/Graphics/Renderer.h>

namespace Nt {
	FTText::FTText(NotNull<FTFont*> pFont) :
		m_pFont(pFont) {
		_Initialize();
	}

	FTText::FTText(NotNull<FTFont*> pFont, String text):
		m_pFont(pFont),
		m_Text(std::move(text))
	{
		_Initialize();
	}

	void FTText::Render(NotNull<Renderer*> pRenderer) const {
		if (m_Text.empty())
			return;

		Float2D prevSize;
		pRenderer->MatrixWorldPush();
		pRenderer->Translate(GetPosition());

		const uInt length = m_Text.length();
		for (uInt i = 0; i < length; ++i) {
			const FTFont::Character& character =
				m_pFont->GetCharacter(m_Text[i]);

			if (character.pGlyph == nullptr) {
				pRenderer->UnbindTexture();
				continue;
			}

			const Float2D size = Float2D(character.Size) * GetSize().xy / 10.f;

			pRenderer->BindTexture(character.pGlyph.get());
			pRenderer->Scale(Float3D(size, 0.f));
			pRenderer->Render(&m_Mesh);
			pRenderer->Scale(Float3D(-size, 0.f));
			pRenderer->Translate({ size.x, 0.f, 0.f });
		}

		pRenderer->MatrixWorldPop();
	}

	void FTText::SetText(String text) noexcept {
		m_Text = std::move(text);
	}

	Mesh& FTText::GetMesh() noexcept {
		return m_Mesh;
	}

	void FTText::_Initialize() {
		SetSize({ 1.f, 1.f, 1.f });

		if (!m_pFont->IsLoaded())
			Raise("Font is unloaded");
	}

	void FTText::Render(NotNull<Renderer*> pRenderer, const uInt& offset, const uInt& verticesCount) const {
		(void)pRenderer;
		(void)offset;
		(void)verticesCount;
	}
}