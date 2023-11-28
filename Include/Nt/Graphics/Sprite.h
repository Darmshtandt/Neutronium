#pragma once

namespace Nt {
	class Texture;

	class Sprite : public IObject {
	protected:
		NT_API Sprite();

	public:
		NT_API Sprite(const uInt& TextureIndex, const Float2D& Size = { 1.f, 1.f }, const Float4D& Color = Colors::White);
		NT_API Sprite(const Texture& NewTexture, const Float2D& Size = { 1.f, 1.f }, const Float4D& Color = Colors::White);
		NT_API Sprite(const Sprite& NewSprite);
		NT_API ~Sprite();

		NT_API void Render(Renderer* pRenderer) const override;


		NT_API const std::unique_ptr<Mesh>& GetMeshPtr() const noexcept;
		NT_API FloatRect GetTextureRect() const noexcept;
		NT_API uInt GetTextureIndex() const noexcept;

		NT_API void SetTextureRect(const FloatRect& TextureRect);
		NT_API void SetTexture(const Texture& NewTexture);
		NT_API void SetTexture(const uInt& TextureIndex);

		NT_API void SetSize(const Float3D& Size);

	private:
		FloatRect m_TextureRect;
		std::unique_ptr<Mesh> m_pMesh;
		Texture* m_pTexture;
		uInt m_TextureIndex;

	private:
		NT_API void _CheckTexture() const;
		NT_API void _UpdateTexCoords() const;
	};
}