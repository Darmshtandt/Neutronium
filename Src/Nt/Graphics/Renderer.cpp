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

#include <Nt/Graphics/System/HandleWindow.h>
#include <Nt/Graphics/Renderer.h>

namespace Nt {
	Renderer::Renderer(const Bool& isEnabled3D) noexcept :
		m_IsEnabled3D(isEnabled3D)
	{
	}

	Renderer::~Renderer() {
		if (m_hwnd != nullptr)
			ReleaseDC(m_hwnd, m_hdc);

		if (m_hContext != nullptr)
			wglDeleteContext(m_hContext);
	}

	void Renderer::Resize() {
		Resize(GetWindowRect(m_hwnd).RightBottom);
	}
	void Renderer::Resize(const uInt2D& size) {
		m_ProjectionConfig.Rect.RightBottom = size;
		Resize(m_ProjectionConfig.Rect);
	}
	void Renderer::Resize(FloatRect rect) {
		SetViewport(rect);

		rect.Right /= m_Zoom;
		rect.Bottom /= m_Zoom;

		switch (m_ProjectionConfig.Type) {
		case ProjectionType::NONE:
			m_Matrices.Projection.MakeIdentity();
			break;

		case ProjectionType::ORTHO:
			SetOrthoProjection(rect, m_ProjectionConfig.Near, m_ProjectionConfig.Far);
			break;

		case ProjectionType::ORTHO2D:
			SetOrtho2DProjection(rect);
			break;

		case ProjectionType::PERSPECTIVE:
			SetPerspectiveProjection(
				m_ProjectionConfig.FOV, rect.Right / rect.Bottom, m_ProjectionConfig.Near, m_ProjectionConfig.Far);
			break;
		}
	}

	void Renderer::SetClearColor(const Float4D& clearColor) const noexcept {
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}
	void Renderer::Clear() {
		CheckInitialization();

		glClear(GL_COLOR_BUFFER_BIT);
		if (m_IsEnabled3D)
			glClear(GL_DEPTH_BUFFER_BIT);
	}
	void Renderer::Display() {
		CheckInitialization();

		m_FrameTimeMs = Float(m_LoopTimeStamp.GetElapsedTimeMs());
		if (m_FrameTimeMs == 0.f)
			m_FrameTimeMs = FLT_EPSILON;

		m_LoopTimeStamp.Restart();

		const Int delayTimeMs = (1000 / m_FPSLimit) - Int(m_FrameTimeMs);
		if (delayTimeMs > 0)
			Sleep(delayTimeMs);
		else
			Sleep(1);

		++m_FPSCounter;
		if (m_FPSTimer.GetElapsedTimeMs() > 1000ull) {
			m_FPS = m_FPSCounter;
			m_FPSCounter = 0;
			m_FPSTimer.Restart();
		}

		SwapBuffers(m_hdc);
	}
	void Renderer::Render(NotNull<const Mesh*> pMesh) const {
		RenderInstanced(pMesh, 1);
	}
	void Renderer::Render(NotNull<const Mesh*> pMesh, const uInt& offset, const uInt& verticesCount) const {
		RenderInstanced(pMesh, offset, verticesCount, 1);
	}
	void Renderer::RenderInstanced(NotNull<const Mesh*> pMesh, const uInt& count) const {
		RenderInstanced(pMesh, 0, pMesh->GetVerticesCount(), count);
	}
	void Renderer::RenderInstanced(NotNull<const Mesh*> pMesh, const uInt& offset, const uInt& verticesCount, const uInt& count) const {
		CheckInitialization();

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->Use();

		pMesh->Bind();
		if (pMesh->GetIndices().empty())
			glDrawArraysInstanced(uInt(m_DrawingMode), offset, verticesCount, count);
		else
			glDrawElementsInstanced(uInt(m_DrawingMode), verticesCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(Int)), count);
	}


	void Renderer::Translate(const Float3D& offset) {
		if (offset == 0.f)
			return;

		m_Matrices.World.Translate(offset);
		_ApplyWorldMatrix();
	}
	void Renderer::Scale(const Float3D& size) {
		if (size == 0.f)
			return;

		m_Matrices.World.Scale(size);
		_ApplyWorldMatrix();
	}
	void Renderer::Rotate(const Float3D& angles) {
		if (angles == 0.f)
			return;

		m_Matrices.World.Rotate(angles);
		_ApplyWorldMatrix();
	}
	void Renderer::Rotate2D(const Float& angle) {
		if (angle == 0.f)
			return;

		m_Matrices.World.Rotate({ 0.f, 0.f, angle });
		_ApplyWorldMatrix();
	}

	void Renderer::Transform(const Float3D& offset, const Float3D& origin, const Float3D& angles, const Float3D& angleOrigin) {
		m_Matrices.World.Translate(offset);
		m_Matrices.World.Rotate(angleOrigin);
		m_Matrices.World.Translate(origin);
		m_Matrices.World.Rotate(angles);
		_ApplyWorldMatrix();
	}

	void Renderer::RotateAroundOrigin(const Float3D& origin, const Float3D& angles) {
		Translate(origin);
		Rotate(angles);
		Translate(-origin);

		_ApplyWorldMatrix();
	}
	void Renderer::Rotate2DAroundOrigin(const Float3D& origin, const Float& angle) {
		Translate(origin);
		Rotate2D(angle);
		Translate(-origin);

		_ApplyWorldMatrix();
	}

	void Renderer::BindTexture(const Texture* pTexture) {
		if (pTexture != nullptr) {
			pTexture->Bind();
			m_IsBindedTexture = true;
		}
		else if (m_IsBindedTexture) {
			glBindTexture(GL_TEXTURE_2D, 0);
			m_IsBindedTexture = false;
		}

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->SetUniform("fTexture", m_IsBindedTexture);
	}
	void Renderer::UnbindTexture() {
		if (!m_IsBindedTexture)
			return;

		glBindTexture(GL_TEXTURE_2D, 0);
		m_IsBindedTexture = false;

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->SetUniform("fTexture", m_IsBindedTexture);
	}
	void Renderer::BindMesh(Mesh* pMesh) {
		if (pMesh != nullptr)
			pMesh->Bind();
		else
			glBindVertexArray(0);
	}
	void Renderer::UnbindMesh() {
		glBindVertexArray(0);
	}

	void Renderer::Hint(const HintTarget& target, const HintMode& mode) const noexcept {
		glHint(uInt(target), uInt(mode));
	}

	void Renderer::EnableMultisample() const noexcept {
		glEnable(GL_MULTISAMPLE);
	}

	void Renderer::DisableMultisample() const noexcept {
		glDisable(GL_MULTISAMPLE);
	}

	void Renderer::EnableDepthBuffer() const noexcept {
		glEnable(GL_DEPTH_TEST);
	}
	void Renderer::DisableDepthBuffer() const noexcept {
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::EnableDepthMask() const noexcept {
		glDepthMask(GL_TRUE);
	}

	void Renderer::DisableDepthMask() const noexcept {
		glDepthMask(GL_FALSE);
	}

	void Renderer::SetCullFace(const CullFace& mode) noexcept {
		if (m_CullFace == mode)
			return;

		m_CullFace = mode;
		if (mode == CullFace::NONE) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glEnable(GL_CULL_FACE);
			glCullFace(uInt(mode));
		}
	}
	void Renderer::SetViewport(const IntRect& rect) {
		m_ViewportRect = rect;
		glViewport(rect.Left, rect.Top, rect.Right, rect.Bottom);
	}

	void Renderer::MatricesPush() noexcept {
		m_CachedMatrices = m_Matrices;
	}
	void Renderer::MatrixWorldPush() noexcept {
		m_CachedMatrices.World = m_Matrices.World;
	}
	void Renderer::MatrixViewPush() noexcept {
		m_CachedMatrices.View = m_Matrices.View;
	}
	void Renderer::MatrixProjectionPush() noexcept {
		m_CachedMatrices.Projection = m_Matrices.Projection;
	}

	void Renderer::MatricesPop() noexcept {
		m_Matrices = m_CachedMatrices;
		_ApplyWorldMatrix();
		_ApplyViewMatrix();
		_ApplyProjectionMatrix();
	}
	void Renderer::MatrixWorldPop() noexcept {
		m_Matrices.World = m_CachedMatrices.World;
		_ApplyWorldMatrix();
	}
	void Renderer::MatrixViewPop() noexcept {
		m_Matrices.View = m_CachedMatrices.View;
		_ApplyViewMatrix();
	}
	void Renderer::MatrixProjectionPop() noexcept {
		m_Matrices.Projection = m_CachedMatrices.Projection;
		_ApplyProjectionMatrix();
	}


	void Renderer::SetOrthoProjection(FloatRect rect, const Float& orthoNear, const Float& orthoFar) {
		if (rect.Left == rect.Right || rect.Top == rect.Bottom) {
			Log::Instance().Warning("SetOrthoProjection: Invalid value");
			return;
		}

		rect.LeftTop *= m_Zoom;
		rect.RightBottom /= m_Zoom;

		m_ProjectionConfig = { };
		m_ProjectionConfig.Rect = rect;
		m_ProjectionConfig.Near = orthoNear;
		m_ProjectionConfig.Far = orthoFar;

		if (orthoNear == -1.f && orthoFar == 1.f)
			m_ProjectionConfig.Type = ProjectionType::ORTHO2D;
		else
			m_ProjectionConfig.Type = ProjectionType::ORTHO;

		m_Matrices.Projection._11 = 2.f / (rect.Right - rect.Left);
		m_Matrices.Projection._22 = 2.f / (rect.Top - rect.Bottom);
		m_Matrices.Projection._33 = 2.f / (orthoFar - orthoNear);
		m_Matrices.Projection._44 = 1.f;

		m_Matrices.Projection._14 = -(rect.Right + rect.Left) / (rect.Right - rect.Left);
		m_Matrices.Projection._24 = -(rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
		m_Matrices.Projection._34 = -(orthoFar + orthoNear) / (orthoFar - orthoNear);

		_ApplyProjectionMatrix();
	}
	void Renderer::SetOrtho2DProjection(const FloatRect& rect) {
		SetOrthoProjection(rect, -1.f, 1.f);
	}
	void Renderer::SetPerspectiveProjection(const Float& fov, const Float& aspect, const Float& _near, const Float& _far) {
		m_ProjectionConfig.FOV = fov;
		m_ProjectionConfig.Aspect = aspect;
		m_ProjectionConfig.Near = _near;
		m_ProjectionConfig.Far = _far;
		m_ProjectionConfig.Type = ProjectionType::PERSPECTIVE;

		const Float tanFOV = tanf(fov / 2.f * RADf);

		m_Matrices.Projection.MakeIdentity();
		m_Matrices.Projection._11 = 1.f / (tanFOV * aspect * m_Zoom);
		m_Matrices.Projection._22 = (1.f / tanFOV);
		m_Matrices.Projection._33 = -(_far + _near) / (_far - _near);
		m_Matrices.Projection._34 = -(2.f * _far * _near) / (_far - _near);
		m_Matrices.Projection._43 = -1.f;
		m_Matrices.Projection._44 = 0.f;

		_ApplyProjectionMatrix();
	}

	void Renderer::SetLineWidth(const Float& width) noexcept {
		glLineWidth(width);
	}

	void Renderer::CheckInitialization() const {
		if (!m_IsInitialized)
			Raise("Renderer not initialized");
	}
	Shader* Renderer::GetShaderPtr() const noexcept {
		return m_ShaderPtr;
	}
	Matrix4x4 Renderer::GetWorld() const noexcept {
		return m_Matrices.World;
	}
	Matrix4x4 Renderer::GetView() const noexcept {
		return m_Matrices.View;
	}
	Matrix4x4 Renderer::GetProjection() const noexcept {
		return m_Matrices.Projection;
	}
	Float4D Renderer::GetColor() const noexcept {
		return m_Color;
	}
	uIntRect Renderer::GetViewportRect() const noexcept {
		return m_ViewportRect;
	}
	CullFace Renderer::GetCullFace() const noexcept {
		return m_CullFace;
	}
	DepthMode Renderer::GetDepthMode() const noexcept {
		return m_DepthMode;
	}
	Renderer::DrawingMode Renderer::GetDrawingMode() const noexcept {
		return m_DrawingMode;
	}
	uInt Renderer::GetFPSLimit() const noexcept {
		return m_FPSLimit;
	}
	uInt Renderer::GetFPS() const noexcept {
		return m_FPS;
	}
	Float Renderer::GetFrameTimeMs() const noexcept {
		return m_FrameTimeMs;
	}
	Float Renderer::GetFrameTimeSec() const noexcept {
		if (m_FrameTimeMs == FLT_EPSILON)
			return FLT_EPSILON;
		return m_FrameTimeMs / 1000.f;
	}
	Float Renderer::GetZoom() const noexcept {
		return m_Zoom;
	}

	Bool Renderer::IsBindedTexture() const noexcept {
		return m_IsBindedTexture;
	}

	Bool Renderer::IsInitialized() const noexcept {
		return m_IsInitialized;
	}
	Bool Renderer::IsEnabled3D() const noexcept {
		return m_IsEnabled3D;
	}

	void Renderer::SetFPSLimit(const uInt& fpsLimit) noexcept {
		m_FPSLimit = fpsLimit;
	}
	void Renderer::SetProjection(const Matrix4x4& projection) {
		m_Matrices.Projection = projection;
		_ApplyProjectionMatrix();
	}
	void Renderer::SetWorld(const Matrix4x4& world) {
		m_Matrices.World = world;
		_ApplyWorldMatrix();
	}
	void Renderer::SetView(const Matrix4x4& view) {
		m_Matrices.View = view;
		_ApplyViewMatrix();
	}
	void Renderer::SetColor(const Float4D& color) {
		m_Color = color;
		_ApplyRenderColor();
	}
	void Renderer::SetZoom(const Float& zoom) noexcept {
		m_Zoom = zoom;
	}
	void Renderer::SetDepthMode(const DepthMode& mode) noexcept {
		if (m_DepthMode != mode) {
			m_DepthMode = mode;
			glDepthFunc(uInt(m_DepthMode));
		}
	}
	void Renderer::SetDrawingMode(const DrawingMode& mode) noexcept {
		m_DrawingMode = mode;
	}
	void Renderer::SetShader(Shader* pShader) {
		m_ShaderPtr = pShader;

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->Use();

		_ApplyProjectionMatrix();
		_ApplyWorldMatrix();
		_ApplyViewMatrix();
		_ApplyRenderColor();
	}

	void Renderer::_Initialize(WindowID hwnd) {
		if (m_hContext != nullptr) {
			Log::Instance().Warning("Context already created");
			return;
		}

		m_hwnd = hwnd;
		m_hdc = GetDC(m_hwnd);

		PIXELFORMATDESCRIPTOR PFD = { };
		PFD.nSize = sizeof(PFD);
		PFD.nVersion = 1;
		PFD.cAlphaBits = 8;
		PFD.cAlphaShift = 8;
		PFD.cColorBits = 32;
		PFD.iLayerType = PFD_MAIN_PLANE;
		PFD.iPixelType = PFD_TYPE_RGBA;
		PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

		if (m_IsEnabled3D) {
			PFD.cDepthBits = 24;
			PFD.cStencilBits = 8;
		}

		const Int pixelFormat = ChoosePixelFormat(m_hdc, &PFD);
		if (pixelFormat == 0)
			Raise("Failed ChoosePixelFormat()");
		if (!SetPixelFormat(m_hdc, pixelFormat, &PFD))
			Raise("Failed SetPixelFormat()");
		if (DescribePixelFormat(m_hdc, pixelFormat, sizeof(PFD), &PFD) == 0)
			Raise("Failed DescribePixelFormat()");

		m_hContext = wglCreateContext(m_hdc);
		wglMakeCurrent(m_hdc, m_hContext);

		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_IsEnabled3D)
			EnableDepthBuffer();

		m_Matrices.Projection.MakeIdentity();
		m_Matrices.World.MakeIdentity();
		m_Matrices.View.MakeIdentity();

		RECT ClientRect;
		GetWindowRect(m_hwnd, &ClientRect);
		Resize(uInt2D(ClientRect.right, ClientRect.bottom));

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			Raise("Failed to initialize GLEW");

		m_IsInitialized = true;
	}
	void Renderer::_ApplyProjectionMatrix() const {
		if (m_ShaderPtr != nullptr) {
			m_ShaderPtr->SetUniformMatrix4x4("Projection", UNIFORM_FLOAT, m_Matrices.Projection);
		}
		else {
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(m_Matrices.Projection.Matrix);
			glMatrixMode(GL_MODELVIEW);
		}
	}
	void Renderer::_ApplyWorldMatrix() const {
		if (m_ShaderPtr != nullptr) {
			m_ShaderPtr->SetUniformMatrix4x4("World", UNIFORM_FLOAT, m_Matrices.World);
		}
		else {
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((m_Matrices.View * m_Matrices.World).Matrix);
		}
	}
	void Renderer::_ApplyViewMatrix() const {
		if (m_ShaderPtr != nullptr) {
			m_ShaderPtr->SetUniformMatrix4x4("View", UNIFORM_FLOAT, m_Matrices.View);
		}
		else {
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((m_Matrices.View * m_Matrices.World).Matrix);
		}
	}
	void Renderer::_ApplyRenderColor() const {
		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->SetUniform<Float4D>("RenderColor", m_Color);
		else
			glColor4f(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	}
}