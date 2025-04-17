#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Colors.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Shader.h>

namespace Nt {
	enum class CullFace : uInt {
		NONE = 0x0000,
		FRONT = 0x0404,
		BACK = 0x0405,
		FRONT_AND_BACK = 0x0408
	};

	class Renderer {
	private:
		enum class ProjectionType {
			NONE,
			ORTHO,
			ORTHO2D,
			PERSPECTIVE,
		};

		struct _ProjectionConfig {
			ProjectionType Type = ProjectionType::NONE;
			FloatRect Rect = { };
			Float Aspect = 0.f;
			Float Near = 0.01f;
			Float FOV = 60.f;
			Float Far = 1000.f;
		};

		struct _Matrices {
			Matrix4x4 World;
			Matrix4x4 View;
			Matrix4x4 Projection;
		};

	public:
		enum class DrawingMode : uInt {
			POINTS = 0x0000,
			LINE_STRIP = 0x0003,
			LINE_LOOP = 0x0002,
			LINES = 0x0001,
			LINE_STRIP_ADJACENCY = 0x000B,
			LINES_ADJACENCY = 0x000A,
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN = 0x0006,
			TRIANGLES = 0x0004,
			TRIANGLE_STRIP_ADJACENCY = 0x000D,
			TRIANGLES_ADJACENCY = 0x000C,
			QUADS = 0x0007,
			QUAD_STRIP = 0x0008,
			POLYGON = 0x0009
		};

	public:
		NT_API Renderer(const Bool& isEnabled3D) noexcept;

		NT_API void Resize();
		NT_API void Resize(const uInt2D& size);
		NT_API void Resize(FloatRect rect);

		NT_API void SetClearColor(const Float4D& clearColor) const noexcept;
		NT_API void Clear();
		NT_API void Display();

		NT_API void Render(const Mesh* pMesh) const;
		NT_API void Render(const Mesh* pMesh, const uInt& offset, const uInt& verticesCount) const;
		NT_API void RenderInstanced(const Mesh* pMesh, const uInt& count) const;
		NT_API void RenderInstanced(const Mesh* pMesh, const uInt& offset, const uInt& verticesCount, const uInt& count) const;

		NT_API void Translate(const Float3D& offset);
		NT_API void Rotate(const Float3D& angles);
		NT_API void Rotate2D(const Float& angle);
		NT_API void Transform(const Float3D& offset, const Float3D& origin, const Float3D& angles, const Float3D& angleOrigin);

		NT_API void RotateAroundOrigin(const Float3D& origin, const Float3D& angles);
		NT_API void Rotate2DAroundOrigin(const Float3D& origin, const Float& angle);

		NT_API void BindTexture(const Texture& texture);
		NT_API void UnbindTexture();
		NT_API void BindMesh(const Mesh& mesh);
		NT_API void UnbindMesh();

		NT_API void EnableDepthBuffer() const noexcept;
		NT_API void DisableDepthBuffer() const noexcept;

		NT_API void SetCullFace(const CullFace& mode) const noexcept;
		NT_API void SetViewport(const Nt::IntRect& rect);

		NT_API void MatricesPush() noexcept;
		NT_API void MatrixWorldPush() noexcept;
		NT_API void MatrixViewPush() noexcept;
		NT_API void MatrixProjectionPush() noexcept;

		NT_API void MatricesPop() noexcept;
		NT_API void MatrixWorldPop() noexcept;
		NT_API void MatrixViewPop() noexcept;
		NT_API void MatrixProjectionPop() noexcept;

		NT_API void SetOrthoProjection(FloatRect rect, const Float& orthoNear, const Float& orthoFar);
		NT_API void SetOrtho2DProjection(const FloatRect& rect);
		NT_API void SetPerspectiveProjection(const Float& fov, const Float& aspect, const Float& _near, const Float& _far);

		NT_API void SetLineWidth(const Float& width) noexcept;

		NT_API void CheckInitialization() const;

		NT_API Shader* GetShaderPtr() const noexcept;
		NT_API Matrix4x4 GetWorld() const noexcept;
		NT_API Matrix4x4 GetView() const noexcept;
		NT_API Matrix4x4 GetProjection() const noexcept;
		NT_API Float4D GetColor() const noexcept;
		NT_API uIntRect GetViewportRect() const noexcept;
		NT_API DrawingMode GetDrawingMode() const noexcept;
		NT_API uInt GetFPSLimit() const noexcept;
		NT_API uInt GetFPS() const noexcept;
		NT_API uInt GetFrameTime() const noexcept;
		NT_API Float GetZoom() const noexcept;
		NT_API Bool IsInitialized() const noexcept;
		NT_API Bool IsEnabled3D() const noexcept;

		NT_API void SetFPSLimit(const uInt& fpsLimit) noexcept;
		NT_API void SetProjection(const Matrix4x4& projection);
		NT_API void SetWorld(const Matrix4x4& world);
		NT_API void SetView(const Matrix4x4& view);
		NT_API void SetColor(const Float4D& color);
		NT_API void SetZoom(const Float& zoom) noexcept;
		NT_API void SetDrawingMode(const DrawingMode& mode) noexcept;
		NT_API void SetCurrentShader(Shader* pShader);

	protected:
#ifdef _WINDEF_
		NT_API void _Initialize(HWND hwnd);
#endif

	private:
		_Matrices m_Matrices;
		_Matrices m_CachedMatrices;
		_ProjectionConfig m_ProjectionConfig;
		Float4D m_Color = Colors::White;

		uIntRect m_ViewportRect;
		Shader* m_ShaderPtr = nullptr;

#ifdef _WINDEF_
		HWND m_hwnd = nullptr;
#endif
		DrawingMode m_DrawingMode = DrawingMode::TRIANGLES;

		Int m_FPSLimit = 120;
		Int m_FPSCounter = 0;
		Int m_FPS = 0;
		Int m_FrameTime = 0;
		Timer m_LoopTimeStamp;
		Timer m_FPSTimer;

		Float m_Zoom = 1.f;
		Bool m_IsEnabled3D;
		Bool m_IsInitialized = false;

	private:
		NT_API void _ApplyProjectionMatrix() const;
		NT_API void _ApplyWorldMatrix() const;
		NT_API void _ApplyViewMatrix() const;
		NT_API void _ApplyRenderColor() const;
	};
}