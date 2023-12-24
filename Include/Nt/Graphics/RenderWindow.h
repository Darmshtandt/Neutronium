#pragma once

namespace Nt {
	class RenderWindow : public Renderer, public Window {
	public:
		RenderWindow(const Bool& IsEnabled3D) noexcept :
			Renderer(IsEnabled3D),
			m_CurrentCameraPtr(nullptr)
		{ }
		RenderWindow(const IntRect& WindowRect, const String& Name, const Bool& IsEnabled3D) :
			Renderer(IsEnabled3D),
			m_CurrentCameraPtr(nullptr)
		{
			Create(WindowRect, Name);
		}
		RenderWindow(const Int2D& Size, const String& Name, const Bool& IsEnabled3D) :
			Renderer(IsEnabled3D),
			m_CurrentCameraPtr(nullptr)
		{
			Create(Size, Name);
		}
		~RenderWindow() {
			SAFE_DELETE(&m_DefaultShader);
		}

		void Create(const Int2D& Size, const String& Name) {
			Window::Create(Size, Name);
		}
		void Create(const IntRect& WindowRect, const String& Name) {
			Window::Create(WindowRect, Name);
		}

		NT_API void Update();

		void Resize(const uInt2D& size) {
			Renderer::Resize(size);
			Window::SetSize(size);
		}

		void SetPerspectiveProjection(const Float& FOV, const Float& Near, const Float& Far) {
			const Rect<Int> WindowRect = Window::GetClientRect();
			Renderer::SetPerspectiveProjection(
				FOV,
				static_cast<Float>(WindowRect.Right) / static_cast<Float>(WindowRect.Bottom),
				Near,
				Far);
		}
		void SetOrthoProjection(const Float& Near, const Float& Far) {
			const Float2D WindowSize = Window::GetClientRect().RightBottom;
			const FloatRect Rect(0.f, WindowSize.y / WindowSize.x, WindowSize.x / WindowSize.y, 0.f);
			Renderer::SetOrthoProjection(Rect, Near, Far);
		}
		void SetOrtho2DProjection(const FloatRect& Rect) {
			Renderer::SetOrthoProjection(Rect, -1.f, 1.f);
		}
		void SetOrtho2DProjection() {
			SetOrthoProjection(-1.f, 1.f);
		}
		void SetCurrentCamera(Camera* CurrentCameraPtr) {
			m_CurrentCameraPtr = CurrentCameraPtr;
		}

		using Renderer::SetOrthoProjection;
		using Renderer::SetOrtho2DProjection;
		using Renderer::SetPerspectiveProjection;

	private:
		Camera* m_CurrentCameraPtr;
		Shader* m_DefaultShader;

	private:
		using Renderer::Resize;
		using Window::SetSize;

		void _InitializeDefaultShader() {
			const cString VertexShaderCode = R"(
					#version 400 compatibility	
							
					uniform mat4 Projection;	
					uniform mat4 World;			
					uniform mat4 View;			
							
					layout(location = 0) in vec4 Position;
					layout(location = 1) in vec4 Normal;
					layout(location = 2) in vec3 Texcoord;
					layout(location = 3) in vec4 Color;

					out vec3 VertexTexCoords;	
							
					void main() {				
						gl_Position = Projection * View * World * Position;
							
						VertexTexCoords = Texcoord;
					}							
				)";

			const cString FragmentShaderCode = R"(
					#version 400

					in vec3 VertexTexCoords;
					uniform vec4 RenderColor;

					out vec4 Color;

					uniform sampler2D ourTexture;

					void main() {
						vec4 TextureColor = texture(ourTexture, vec2(VertexTexCoords));
						Color = TextureColor * RenderColor;
					}	
				)";

			m_DefaultShader = new Shader;
			m_DefaultShader->Initialize();
			m_DefaultShader->DisableStrict();
			m_DefaultShader->CompileCode(Shader::Types::VERTEX, VertexShaderCode);
			m_DefaultShader->CompileCode(Shader::Types::FRAGMENT, FragmentShaderCode);
			m_DefaultShader->Link();

			Renderer::SetCurrentShader(m_DefaultShader);
		}

		void _WMCreate(const CREATESTRUCT* pWindowStruct) override {
			Renderer::_Initialize(Window::GetHandle());
			glewInit();
			_InitializeDefaultShader();
			if (Renderer::IsEnabled3D())
				SetPerspectiveProjection(60.f, 0.01f, 100.f);
			else
				SetOrtho2DProjection();
		}
		void _WMResize(const uInt2D& size) override {
			Renderer::Resize(size);
		}
	};
}