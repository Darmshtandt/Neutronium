#pragma once


namespace Nt {
	enum class CullFace : unsigned {
		NONE,
		FRONT = GL_FRONT,
		BACK = GL_BACK,
		FRONT_AND_BACK = GL_FRONT_AND_BACK
	};

	class Renderer {
	private:
		enum class _ProjectionType {
			NONE,
			ORTHO,
			ORTHO2D,
			PERSPECTIVE,
		};

		struct _ProjectionConfig {
			Float FOV = 60.f;
			Float Aspect = 0.f;
			Float Near = 0.01f;
			Float Far = 1000.f;

			FloatRect Rect;

			_ProjectionType Type = _ProjectionType::NONE;
		};

		struct _Matrices {
			Matrix4x4 World;
			Matrix4x4 View;
			Matrix4x4 Projection;
		};

	public:
		enum class DrawingMode : unsigned {
			POINTS = GL_POINTS,
			LINE_STRIP = GL_LINE_STRIP,
			LINE_LOOP = GL_LINE_LOOP,
			LINES = GL_LINES,
			LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
			LINES_ADJACENCY = GL_LINES_ADJACENCY,
			TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN = GL_TRIANGLE_FAN,
			TRIANGLES = GL_TRIANGLES,
			TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
			TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
			QUADS = GL_QUADS,
			QUAD_STRIP = GL_QUAD_STRIP,
			POLYGON = GL_POLYGON
		};

	public:
		Renderer(const Bool& isEnabled3D) noexcept :
			m_IsEnabled3D(isEnabled3D),
			m_Zoom(1.f),
			m_FPSLimit(120),
			m_FPSCounter(0),
			m_FrameTime(0),
			m_DrawingMode(DrawingMode::TRIANGLES),
			m_Color(Colors::White),
			m_IsInitialized(false),
			m_ShaderPtr(nullptr),
			m_hwnd(nullptr) {
		}

		void Resize() {
			Resize(GetWindowRect(m_hwnd).RightBottom);
		}
		void Resize(const uInt2D& size) {
			Resize(FloatRect(m_ProjectionConfig.Rect.LeftTop, Float2D(size)));
		}
		void Resize(FloatRect rect) {
			glViewport(uInt(rect.Left), uInt(rect.Top), uInt(rect.Right), uInt(rect.Bottom));

			rect.Right /= m_Zoom;
			rect.Bottom /= m_Zoom;

			switch (m_ProjectionConfig.Type) {
			case _ProjectionType::NONE:
				m_Matrices.Projection.MakeIdentity();
				break;
			case _ProjectionType::ORTHO:
				SetOrthoProjection(rect, m_ProjectionConfig.Near, m_ProjectionConfig.Far);
				break;
			case _ProjectionType::ORTHO2D:
				SetOrtho2DProjection(rect);
				break;
			case _ProjectionType::PERSPECTIVE:
				SetPerspectiveProjection(
					m_ProjectionConfig.FOV, rect.Right / rect.Bottom, m_ProjectionConfig.Near, m_ProjectionConfig.Far);
				break;
			}
		}

		void SetClearColor(const Float4D& clearColor) const noexcept {
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}
		void Clear() {
			CheckInitialization();

			glClear(GL_COLOR_BUFFER_BIT);
			if (m_IsEnabled3D)
				glClear(GL_DEPTH_BUFFER_BIT);
		}
		void Display() {
			CheckInitialization();

			m_FrameTime = m_LoopTimeStamp.GetElapsedTimeMs();
			m_LoopTimeStamp.Restart();

			const uInt delayTimeMs = (1000 / m_FPSLimit) - m_FrameTime;
			if (Int(delayTimeMs) > 0)
				Sleep(delayTimeMs);
			else
				Sleep(1);

			++m_FPSCounter;
			if (m_FPSTimer.GetElapsedTimeMs() > 1000ull) {
				m_FPS = m_FPSCounter;
				m_FPSCounter = 0;
				m_FPSTimer.Restart();
			}

			glFlush();
		}

		void Render(const Mesh* pMesh) const {
			if (!pMesh)
				Raise("Mesh is nullptr");
			RenderInstanced(pMesh, 1);
		}
		void RenderInstanced(const Mesh* pMesh, const GLsizei& Count) const {
			CheckInitialization();
			if (!pMesh)
				Raise("Mesh is nullptr");

			pMesh->Set();
			if (m_ShaderPtr)
				m_ShaderPtr->Use();

			if (pMesh->IsUsedIndexBuffer())
				glDrawElementsInstanced(uInt(m_DrawingMode), pMesh->GetIndices().size(), GL_UNSIGNED_SHORT, nullptr, Count);
			else
				glDrawArraysInstanced(uInt(m_DrawingMode), 0, pMesh->GetVertices().size(), Count);
		}


		void Translate(const Float3D& Offset) {
			if (Offset != Float3D::Zero) {
				m_Matrices.World.Translate(Offset);
				if (m_ShaderPtr)
					m_ShaderPtr->SetUniformMatrix4x4("World", GL_FLOAT, m_Matrices.World);
				else
					_SetGLMatrixModelView();
			}
		}
		void Rotate(const Float3D& Angles) {
			m_Matrices.World.Rotate(Angles);
			_SetWorld();
		}
		void Rotate2D(const Float& Angle) {
			m_Matrices.World.Rotate({ 0.f, 0.f, Angle });
			_SetWorld();
		}

		void RotateAroundOrigin(const Float3D& Origin, const Float3D& Angles) {
			Translate(Origin);
			Rotate(Angles);
			Translate(-Origin);
			_SetWorld();
		}
		void Rotate2DAroundOrigin(const Float3D& Origin, const Float& Angle) {
			Translate(Origin);
			Rotate2D(Angle);
			Translate(-Origin);
			_SetWorld();
		}

		void EnableDepthBuffer() const noexcept {
			glEnable(GL_DEPTH_TEST);
		}
		void DisableDepthBuffer() const noexcept {
			glDisable(GL_DEPTH_TEST);
		}

		void SetCullFace(const CullFace& Mode) const noexcept {
			if (Mode == CullFace::NONE) {
				glDisable(GL_CULL_FACE);
			}
			else {
				glEnable(GL_CULL_FACE);
				glCullFace(uInt(Mode));
			}
		}

		void MatricesPush() noexcept {
			m_CachedMatrices = m_Matrices;
		}
		void MatrixWorldPush() noexcept {
			m_CachedMatrices.World = m_Matrices.World;
		}
		void MatrixViewPush() noexcept {
			m_CachedMatrices.View = m_Matrices.View;
		}
		void MatrixProjectionPush() noexcept {
			m_CachedMatrices.Projection = m_Matrices.Projection;
		}

		void MatricesPop() noexcept {
			m_Matrices = m_CachedMatrices;
		}
		void MatrixWorldPop() noexcept {
			m_Matrices.World = m_CachedMatrices.World;
		}
		void MatrixViewPop() noexcept {
			m_Matrices.View = m_CachedMatrices.View;
		}
		void MatrixProjectionPop() noexcept {
			m_Matrices.Projection = m_CachedMatrices.Projection;
		}


		void SetOrthoProjection(FloatRect rect, const Float& orthoNear, const Float& orthoFar) {
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
				m_ProjectionConfig.Type = _ProjectionType::ORTHO2D;
			else
				m_ProjectionConfig.Type = _ProjectionType::ORTHO;

			m_Matrices.Projection._11 = 2.f / (rect.Right - rect.Left);
			m_Matrices.Projection._22 = 2.f / (rect.Top - rect.Bottom);
			m_Matrices.Projection._33 = -2.f / (orthoFar - orthoNear);
			m_Matrices.Projection._44 = 1.f;

			m_Matrices.Projection._14 = -(rect.Right + rect.Left) / (rect.Right - rect.Left);
			m_Matrices.Projection._24 = -(rect.Top + rect.Bottom) / (rect.Top - rect.Bottom);
			m_Matrices.Projection._34 = -(orthoFar + orthoNear) / (orthoFar - orthoNear);

			_SetProjection();
		}
		void SetOrtho2DProjection(const FloatRect& Rect) {
			SetOrthoProjection(Rect, -1.f, 1.f);
		}
		void SetPerspectiveProjection(const Float& FOV, const Float& Aspect, const Float& Near, const Float& Far) {
			m_ProjectionConfig.FOV = FOV;
			m_ProjectionConfig.Aspect = Aspect;
			m_ProjectionConfig.Near = Near;
			m_ProjectionConfig.Far = Far;
			m_ProjectionConfig.Type = _ProjectionType::PERSPECTIVE;

			const Float f = 1.f / tanf(FOV * RADf / 2.f);
			m_Matrices.Projection._11 = f / Aspect / m_Zoom;
			m_Matrices.Projection._22 = f;
			m_Matrices.Projection._33 = (Near + Far) / (Near - Far);
			m_Matrices.Projection._34 = (2.f * Near * Far) / (Near - Far);
			m_Matrices.Projection._43 = -1.f;
			m_Matrices.Projection._44 = 0.f;

			_SetProjection();
		}

		void SetLineWidth(const Float& Width) noexcept {
			glLineWidth(Width);
		}

		NT_API void CheckInitialization() const;

		Shader* GetShaderPtr() const noexcept {
			return m_ShaderPtr;
		}
		Matrix4x4 GetWorld() const noexcept {
			return m_Matrices.World;
		}
		Matrix4x4 GetView() const noexcept {
			return m_Matrices.View;
		}
		Matrix4x4 GetProjection() const noexcept {
			return m_Matrices.Projection;
		}
		Float4D GetColor() const noexcept {
			return m_Color;
		}
		DrawingMode GetDrawingMode() const noexcept {
			return m_DrawingMode;
		}
		uInt GetFPSLimit() const noexcept {
			return m_FPSLimit;
		}
		uInt GetFPS() const noexcept {
			return m_FPS;
		}
		uInt GetFrameTime() const noexcept {
			return m_FrameTime;
		}
		Float GetZoom() const noexcept {
			return m_Zoom;
		}
		Bool IsInitialized() const noexcept {
			return m_IsInitialized;
		}
		Bool IsEnabled3D() const noexcept {
			return m_IsEnabled3D;
		}

		void SetFPSLimit(const uInt& FPSLimit) noexcept {
			m_FPSLimit = FPSLimit;
		}
		void SetProjection(const Matrix4x4& Projection) {
			m_Matrices.Projection = Projection;
			_SetProjection();
		}
		void SetWorld(const Matrix4x4& World) {
			m_Matrices.World = World;
			_SetWorld();
		}
		void SetView(const Matrix4x4& View) {
			m_Matrices.View = View;
			_SetView();
		}
		void SetColor(const Float4D& Color) {
			m_Color = Color;
			if (m_ShaderPtr)
				m_ShaderPtr->SetUniformVec4("RenderColor", GL_FLOAT, Color);
			else
				glColor4f(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
		}
		void SetZoom(const Float& Zoom) noexcept {
			m_Zoom = Zoom;
		}
		void SetDrawingMode(const DrawingMode& Mode) noexcept {
			m_DrawingMode = Mode;
		}
		void SetCurrentShader(Shader* ShaderPtr) {
			m_ShaderPtr = ShaderPtr;
			if (m_ShaderPtr) {
				const Bool IsNtExceptsEnabled = g_NtExcepts;
				m_ShaderPtr->Use();
				m_ShaderPtr->SetUniformMatrix4x4("Projection", GL_FLOAT, m_Matrices.Projection);
				m_ShaderPtr->SetUniformMatrix4x4("World", GL_FLOAT, m_Matrices.World);
				m_ShaderPtr->SetUniformMatrix4x4("View", GL_FLOAT, m_Matrices.View);
				m_ShaderPtr->SetUniformVec4("RenderColor", GL_FLOAT, m_Color);
			}
			else {
				glUseProgram(0);
				_SetGLMatrixProjection();
				_SetGLMatrixModelView();
			}
		}

	protected:
		NT_API void _Initialize(HWND hwnd);

	private:
		_Matrices m_Matrices;
		_Matrices m_CachedMatrices;
		_ProjectionConfig m_ProjectionConfig;
		Float4D m_Color;

		Shader* m_ShaderPtr;

		HWND m_hwnd;
		DrawingMode m_DrawingMode;

		uInt m_FPSLimit;
		uInt m_FPSCounter;
		uInt m_FPS;
		uInt m_FrameTime;
		Timer m_LoopTimeStamp;
		Timer m_FPSTimer;

		Float m_Zoom;
		Bool m_IsEnabled3D;
		Bool m_IsInitialized;

	private:
		void _SetProjection() const {
			if (m_ShaderPtr) {
				m_ShaderPtr->SetUniformMatrix4x4("Projection", GL_FLOAT, m_Matrices.Projection);
			}
			else {
				_SetGLMatrixProjection();
				glMatrixMode(GL_MODELVIEW);
			}
		}
		void _SetWorld() const {
			if (m_ShaderPtr)
				m_ShaderPtr->SetUniformMatrix4x4("World", GL_FLOAT, m_Matrices.World);
			else
				_SetGLMatrixModelView();
		}
		void _SetView() const {
			if (m_ShaderPtr)
				m_ShaderPtr->SetUniformMatrix4x4("View", GL_FLOAT, m_Matrices.View);
			else
				_SetGLMatrixModelView();
		}

		void _SetGLMatrixModelView() const noexcept {
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((m_Matrices.View * m_Matrices.World).Matrix);
		}
		void _SetGLMatrixProjection() const noexcept {
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(m_Matrices.Projection.Matrix);
		}
	};
}