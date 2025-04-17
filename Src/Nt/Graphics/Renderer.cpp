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

#include <Nt/Graphics/HandleWindow.h>
#include <Nt/Graphics/Renderer.h>


namespace Nt {
	Renderer::Renderer(const Bool& isEnabled3D) noexcept :
		m_IsEnabled3D(isEnabled3D)
	{
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

		m_FrameTime = Int(m_LoopTimeStamp.GetElapsedTimeMs());
		m_LoopTimeStamp.Restart();

		const Int delayTimeMs = (1000 / m_FPSLimit) - m_FrameTime;
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

		SwapBuffers(GetDC(m_hwnd));
	}
	void Renderer::Render(const Mesh* pMesh) const {
		RenderInstanced(pMesh, 1);
	}
	void Renderer::Render(const Mesh* pMesh, const uInt& offset, const uInt& verticesCount) const {
		RenderInstanced(pMesh, offset, verticesCount, 1);
	}
	void Renderer::RenderInstanced(const Mesh* pMesh, const uInt& count) const {
		if (pMesh == nullptr)
			return;

		if (pMesh->IsUsedIndexBuffer())
			RenderInstanced(pMesh, 0, pMesh->GetIndices().size(), count);
		else
			RenderInstanced(pMesh, 0, pMesh->GetVertices().size(), count);
	}
	void Renderer::RenderInstanced(const Mesh* pMesh, const uInt& offset, const uInt& verticesCount, const uInt& count) const {
		CheckInitialization();
		if (pMesh == nullptr)
			return;

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->Use();

		pMesh->Bind();
		if (pMesh->IsUsedIndexBuffer())
			glDrawElementsInstanced(uInt(m_DrawingMode), verticesCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(Int)), count);
		else
			glDrawArraysInstanced(uInt(m_DrawingMode), offset, verticesCount, count);
	}


	void Renderer::Translate(const Float3D& offset) {
		if (offset == 0.f)
			return;

		m_Matrices.World.Translate(offset);
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
		if (offset == 0.f && origin == 0.f && angles == 0.f && angleOrigin == 0.f)
			return;

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

	void Renderer::BindTexture(const Texture& texture) {
		texture.Bind();
	}
	void Renderer::UnbindTexture() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Renderer::BindMesh(const Mesh& mesh) {
		mesh.Bind();
	}
	void Renderer::UnbindMesh() {
		glBindVertexArray(0);
	}

	void Renderer::EnableDepthBuffer() const noexcept {
		glEnable(GL_DEPTH_TEST);
	}
	void Renderer::DisableDepthBuffer() const noexcept {
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::SetCullFace(const CullFace& mode) const noexcept {
		if (mode == CullFace::NONE) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glEnable(GL_CULL_FACE);
			glCullFace(uInt(mode));
		}
	}
	void Renderer::SetViewport(const Nt::IntRect& rect) {
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
			Log::Warning("SetOrthoProjection: Invalid value");
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
		m_Matrices.Projection._33 = -2.f / (orthoFar - orthoNear);
		m_Matrices.Projection._44 = 1.f;

		m_Matrices.Projection._14 = -(rect.Right + rect.Left) / (rect.Right - rect.Left);
		m_Matrices.Projection._24 = (rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
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
	Renderer::DrawingMode Renderer::GetDrawingMode() const noexcept {
		return m_DrawingMode;
	}
	uInt Renderer::GetFPSLimit() const noexcept {
		return m_FPSLimit;
	}
	uInt Renderer::GetFPS() const noexcept {
		return m_FPS;
	}
	uInt Renderer::GetFrameTime() const noexcept {
		return m_FrameTime;
	}
	Float Renderer::GetZoom() const noexcept {
		return m_Zoom;
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
	void Renderer::SetDrawingMode(const DrawingMode& mode) noexcept {
		m_DrawingMode = mode;
	}
	void Renderer::SetCurrentShader(Shader* pShader) {
		m_ShaderPtr = pShader;

		if (m_ShaderPtr != nullptr)
			m_ShaderPtr->Use();

		_ApplyProjectionMatrix();
		_ApplyWorldMatrix();
		_ApplyViewMatrix();
		_ApplyRenderColor();
	}

	void Renderer::_Initialize(HWND hwnd) {
		if (wglGetCurrentContext() == nullptr) {
			m_hwnd = hwnd;

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

			const HDC hdc = GetDC(m_hwnd);
			const Int pixelFormat = ChoosePixelFormat(hdc, &PFD);

			try {
				if (pixelFormat == 0)
					Raise(L"Failed ChoosePixelFormat()");
				if (!SetPixelFormat(hdc, pixelFormat, &PFD))
					Raise(L"Failed SetPixelFormat()");
				if (DescribePixelFormat(hdc, pixelFormat, sizeof(PFD), &PFD) == 0)
					Raise(L"Failed DescribePixelFormat()");
			}
			catch (const Nt::Error& error) {
				ReleaseDC(m_hwnd, hdc);
				throw error;
			}

			HGLRC hContext = wglCreateContext(hdc);
			wglMakeCurrent(hdc, hContext);

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
				Raise(L"Failed to initialize GLEW");

			m_IsInitialized = true;
		}
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
			m_ShaderPtr->SetUniform4D("RenderColor", m_Color);
		else
			glColor4f(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	}
}