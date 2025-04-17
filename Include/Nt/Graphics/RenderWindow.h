#pragma once

#include <Nt/Graphics/Objects/Camera.h>
#include <Nt/Graphics/Window.h>
#include <Nt/Graphics/Renderer.h>

namespace Nt {
	class RenderWindow : public Renderer, public Window {
	public:
		NT_API RenderWindow(const Bool& isEnabled3D) noexcept;
		NT_API RenderWindow(const IntRect& windowRect, const String& name, const Bool& isEnabled3D);
		NT_API RenderWindow(const Int2D& size, const String& name, const Bool& isEnabled3D);
		NT_API ~RenderWindow();

		NT_API void Create(const Int2D& size, const String& name);
		NT_API void Create(const IntRect& windowRect, const String& name);

		NT_API void Update();

		NT_API void Resize(const uInt2D& size);

		NT_API void SetPerspectiveProjection(const Float& fov, const Float& _near, const Float& _far);
		NT_API void SetOrthoProjection(const Float& _near, const Float& _far);
		NT_API void SetOrtho2DProjection(const FloatRect& rect);
		NT_API void SetOrtho2DProjection();
		NT_API void SetCurrentCamera(Camera* pCamera);

	private:
		Camera* m_CameraPtr = nullptr;
		Shader* m_DefaultShader = nullptr;

	private:
		using Renderer::Resize;
		using Window::SetSize;

		NT_API void _InitializeDefaultShader();

#ifdef _WINDEF_
		NT_API void _Creation([[maybe_unused]] const CREATESTRUCT* pWindowStruct) override;

		NT_API void _Resize(const uInt2D& size) override;

#endif
	};
}