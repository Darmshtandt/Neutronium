// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOGDI
#undef NOMSG

#include <Windows.h>
#include <shobjidl.h>
#include <GL/GLEW.h>
#include <GL/GL.h>
#include <iostream>


#include <Nt/Graphics/RenderWindow.h>

namespace Nt {
	RenderWindow::RenderWindow(const Bool& isEnabled3D) noexcept :
		Renderer(isEnabled3D)
	{
	}
	RenderWindow::RenderWindow(const IntRect& windowRect, const String& name, const Bool& isEnabled3D) :
		Renderer(isEnabled3D)
	{
		RenderWindow::Create(windowRect, name);
	}
	RenderWindow::RenderWindow(const Int2D& size, const String& name, const Bool& isEnabled3D) :
		Renderer(isEnabled3D)
	{
		RenderWindow::Create(size, name);
	}
	RenderWindow::~RenderWindow() {
		SAFE_DELETE(&m_DefaultShader);
	}

	void RenderWindow::Create(const Int2D& size, const String& name) {
		Window::Create(size, name);
	}
	void RenderWindow::Create(const IntRect& windowRect, const String& name) {
		Window::Create(windowRect, name);
	}

	void RenderWindow::RenderWindow::Update() {
		if (m_CameraPtr != nullptr && m_CameraPtr->IsChanged()) {
			m_CameraPtr->Update();
			Renderer::SetView(m_CameraPtr->GetView());
		}
	}

	void RenderWindow::Resize(const uInt2D& size) {
		Renderer::Resize(size);
		Window::SetSize(size);
	}

	void RenderWindow::SetPerspectiveProjection(const Float& fov, const Float& _near, const Float& _far) {
		const IntRect windowRect = Window::GetClientRect();
		const Float aspect = Float(windowRect.Right) / Float(windowRect.Bottom);

		Renderer::SetPerspectiveProjection(fov, aspect, _near, _far);
	}
	void RenderWindow::SetOrthoProjection(const Float& _near, const Float& _far) {
		const Float2D windowSize = Window::GetClientRect().RightBottom;
		const Float aspect = (windowSize.x / windowSize.y);

		const Nt::FloatRect orthoRect = { 0.f, 0.f, aspect * windowSize.x, -windowSize.y };
		Renderer::SetOrthoProjection(orthoRect, _near, _far);
	}
	void RenderWindow::SetOrtho2DProjection(const FloatRect& rect) {
		Renderer::SetOrthoProjection(rect, -1.f, 1.f);
	}
	void RenderWindow::SetOrtho2DProjection() {
		SetOrthoProjection(-1.f, 1.f);
	}
	void RenderWindow::SetCurrentCamera(Camera* pCamera) {
		m_CameraPtr = pCamera;
	}

	void RenderWindow::_InitializeDefaultShader() {
		const cString vertexShaderCode = R"(
					#version 400 compatibility	
							
					uniform mat4 Projection;	
					uniform mat4 World;			
					uniform mat4 View;			
							
					layout(location = 0) in vec4 Position;
					layout(location = 1) in vec4 Normal;
					layout(location = 2) in vec3 Texcoord;
					layout(location = 3) in vec4 Color;

					out vec3 VertexTexCoords;
					out vec4 VertexColor;
							
					void main() {				
						gl_Position = Projection * View * World * Position;
							
						VertexTexCoords = Texcoord;
						VertexColor = Color;
					}							
				)";

		const cString fragmentShaderCode = R"(
					#version 400

					in vec3 VertexTexCoords;
					in vec4 VertexColor;
					uniform vec4 RenderColor;

					out vec4 Color;

					uniform sampler2D ourTexture;

					void main() {
						vec4 TextureColor = texture(ourTexture, vec2(VertexTexCoords));
						Color = TextureColor * RenderColor * VertexColor;
					}	
				)";

		m_DefaultShader = new Shader;
		m_DefaultShader->Initialize();
		m_DefaultShader->Create();
		m_DefaultShader->DisableStrict();
		m_DefaultShader->CompileCode(Shader::Types::VERTEX, vertexShaderCode);
		m_DefaultShader->CompileCode(Shader::Types::FRAGMENT, fragmentShaderCode);
		m_DefaultShader->Link();

		Renderer::SetCurrentShader(m_DefaultShader);
	}

	void RenderWindow::_Creation([[maybe_unused]] const CREATESTRUCT* pWindowStruct) {
		Renderer::_Initialize(Window::GetHandle());
		_InitializeDefaultShader();

		if (Renderer::IsEnabled3D())
			SetPerspectiveProjection(60.f, 0.01f, 100.f);
		else
			SetOrtho2DProjection();
	}

	void RenderWindow::_Resize(const uInt2D& size) {
		Renderer::Resize(size);
	}
}