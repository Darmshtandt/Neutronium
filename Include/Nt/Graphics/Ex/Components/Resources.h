#pragma once

#include <Nt/Core/Colors.h>
#include <Nt/Graphics/Ex/BaseComponent.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/ResourceHandle.h>
#include <Nt/Graphics/System/FTFont.h>

namespace NtEx {
	using namespace Nt;

	template <class _Ty, Requires(std::is_base_of_v<IResource, _Ty>)>
	class ResourceRenderer {
	public:
		ResourceRenderer() noexcept = default;
		ResourceRenderer(const ResourceRenderer&) noexcept = default;
		ResourceRenderer(ResourceRenderer&&) noexcept = default;
		virtual ~ResourceRenderer() noexcept = default;

		ResourceRenderer& operator = (const ResourceRenderer&) noexcept = default;
		ResourceRenderer& operator = (ResourceRenderer&&) noexcept = default;

		NT_NODISCARD const _Ty* Get() const noexcept {
			return m_Resource.Get();
		}
		NT_NODISCARD Bool IsValid() const noexcept {
			return m_Resource.IsValid();
		}

		const _Ty* SetByPointer(_Ty* pointer) noexcept {
			return m_Resource.SetByPointer(pointer);
		}
		const _Ty* Set(const uInt& index) noexcept {
			return m_Resource.Set(index);
		}

	private:
		ResourceHandle<_Ty> m_Resource;
	};

	class MeshRenderer final : public BaseComponent, public ResourceRenderer<Mesh> {
	public:
		explicit MeshRenderer(Object* pOwner) : BaseComponent(pOwner, Class<MeshRenderer>::ID())
		{
		}
		~MeshRenderer() noexcept override = default;

	public:
		Bool IsVisible = true;
	};

	class TextureRenderer final : public BaseComponent, public ResourceRenderer<Texture> {
	public:
		explicit TextureRenderer(Object* pOwner) :
			BaseComponent(pOwner, Class<TextureRenderer>::ID())
		{
		}
		~TextureRenderer() noexcept override = default;
	};

	class TextureMap final : public BaseComponent {
	public:
		explicit TextureMap(Object* pOwner) :
			BaseComponent(pOwner, Class<TextureMap>::ID())
		{
		}

		Texture Main;
		std::unordered_map<uInt, Texture> Map;
	};

	class Text final : public BaseComponent {
	public:
		explicit Text(Object* pOwner) :
			BaseComponent(pOwner, Class<Text>::ID())
		{
		}
		~Text() noexcept override = default;

		FTFont* pFont;
		Nt::Float3D Color = Nt::Colors::Black;
		std::string Str;
	};
}