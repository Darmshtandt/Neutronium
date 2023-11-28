#pragma once

namespace Nt {
	class Text : public IObject {
	public:
		Text(Font* FontPtr) : 
			m_FontPtr(FontPtr) {
			_Initialize();
		}
		Text(Font* FontPtr, const String& Text) : 
			m_FontPtr(FontPtr),
			m_Text(Text) {
			_Initialize();
		}

		void Render(Renderer* pRenderer) const override {
			if (!pRenderer)
				Raise("Renderer is nullptr");

			if (m_Text != "") {
				Float2D PrevSize = { 0.f, 0.f };
				Float3D Offset = GetPosition();
				pRenderer->Translate(Offset);

				const uInt Length = m_Text.length();
				for (uInt i = 0; i < Length; ++i) {
					Font::Character Char = m_FontPtr->GetCharacter(m_Text[i]);
					glBindTexture(GL_TEXTURE_2D, Char.TextureID);
					const Float2D Size = Float2D(Char.Size) * GetSize() / 10.f;
					if (PrevSize != Size) {
						m_pMesh->SetShape(Geometry::Quad(Size, Colors::White));
						PrevSize = Size;
					}
					pRenderer->Render(m_pMesh.get());
					pRenderer->Translate({ Size.x, 0.f, 0.f });
					Offset.x += Size.x;
				}
				pRenderer->Translate(-Offset);
			}
		}

		void SetText(String Text) noexcept {
			m_Text = Text;
		}

	private:
		String m_Text;
		std::unique_ptr<Mesh> m_pMesh;
		Font* m_FontPtr;

	private:
		void _Initialize() {
			m_pMesh = std::make_unique<Mesh>();
			SetSize(Float3D::Identity);

			if (!m_FontPtr)
				Raise("Font is nullptr");
			if (!m_FontPtr->IsLoaded())
				Raise("Font is unloaded");
		}
	};
}