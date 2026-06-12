#pragma once

#include <Nt/Core/Math/Projections.h>
#include <Nt/Core/Colors.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/System/System.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>
#include <Nt/Graphics/Shader.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Nt {
	enum class CullFace : uInt {
		NONE = 0x0000,
		FRONT = 0x0404,
		BACK = 0x0405,
		FRONT_AND_BACK = 0x0408
	};

	enum class DepthMode {
		NONE = 0x0000,
		NEVER = 0x0200,
		LESS = 0x0201,
		EQUAL = 0x0202,
		LEQUAL = 0x0203,
		GREATER = 0x0204,
		NOT_EQUAL = 0x0205,
		GEQUAL = 0x0206,
		ALWAYS = 0x0207
	};

	enum class HintTarget {
		FOG = 0x0C54,
		GENERATE_MIPMAP = 0x8192,
		LINE_SMOOTH = 0x0C52,
		PERSPECTIVE_CORRECTION = 0x0C50,
		POINT_SMOOTH = 0x0C51,
		POLYGON_SMOOTH = 0x0C53,
		TEXTURE_COMPRESSION = 0x84EF,
		FRAGMENT_SHADER_DERIVATIVE = 0x8B8B,
		MULTISAMPLE_FILTER_NV = 0x8534
	};

	enum class HintMode {
		FASTEST = 0x1101,
		NICEST = 0x1102,
		DONT_CARE = 0x1100
	};

	class NT_API Renderer {
	private:
		enum class ProjectionType : Byte {
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
		explicit Renderer(const Bool& isEnabled3D) noexcept;
		Renderer() = delete;
		Renderer(const Renderer&) = default;
		Renderer(Renderer&&) noexcept = default;
		~Renderer();

		void Resize();
		void Resize(const uInt2D& size);
		void Resize(FloatRect rect);

		void SetClearColor(const Float4D& clearColor) const noexcept;
		void Clear();
		void Display();

		void Render(NotNull<const Mesh*> pMesh) const;
		void Render(NotNull<const Mesh*> pMesh, const uInt& offset, const uInt& verticesCount) const;
		void RenderInstanced(NotNull<const Mesh*> pMesh, const uInt& count) const;
		void RenderInstanced(NotNull<const Mesh*> pMesh, const uInt& offset, const uInt& verticesCount, const uInt& count) const;

		void Translate(const Float3D& offset);
		void Scale(const Float3D& size);
		void Rotate(const Float3D& angles);
		void Rotate2D(const Float& angle);
		void Transform(const Float3D& offset, const Float3D& origin, const Float3D& angles, const Float3D& angleOrigin);

		void RotateAroundOrigin(const Float3D& origin, const Float3D& angles);
		void Rotate2DAroundOrigin(const Float3D& origin, const Float& angle);

		void BindTexture(const Texture* texture);
		void UnbindTexture();
		void BindMesh(Mesh* mesh);
		void UnbindMesh();

		void Hint(const HintTarget& target, const HintMode& mode) const noexcept;

		void EnableMultisample() const noexcept;
		void DisableMultisample() const noexcept;

		void EnableDepthBuffer() const noexcept;
		void DisableDepthBuffer() const noexcept;

		void EnableDepthMask() const noexcept;
		void DisableDepthMask() const noexcept;

		void SetCullFace(const CullFace& mode) noexcept;
		void SetViewport(const IntRect& rect);

		void MatricesPush() noexcept;
		void MatrixWorldPush() noexcept;
		void MatrixViewPush() noexcept;
		void MatrixProjectionPush() noexcept;

		void MatricesPop() noexcept;
		void MatrixWorldPop() noexcept;
		void MatrixViewPop() noexcept;
		void MatrixProjectionPop() noexcept;

		void SetOrthoProjection(FloatRect rect, const Float& orthoNear, const Float& orthoFar);
		void SetOrtho2DProjection(const FloatRect& rect);
		void SetPerspectiveProjection(const Float& fov, const Float& aspect, const Float& _near, const Float& _far);

		void SetLineWidth(const Float& width) noexcept;

		void CheckInitialization() const;

		NT_NODISCARD Shader* GetShaderPtr() const noexcept;
		NT_NODISCARD Matrix4x4 GetWorld() const noexcept;
		NT_NODISCARD Matrix4x4 GetView() const noexcept;
		NT_NODISCARD Matrix4x4 GetProjection() const noexcept;
		NT_NODISCARD Float4D GetColor() const noexcept;
		NT_NODISCARD uIntRect GetViewportRect() const noexcept;
		NT_NODISCARD CullFace GetCullFace() const noexcept;
		NT_NODISCARD DepthMode GetDepthMode() const noexcept;
		NT_NODISCARD DrawingMode GetDrawingMode() const noexcept;
		NT_NODISCARD uInt GetFPSLimit() const noexcept;
		NT_NODISCARD uInt GetFPS() const noexcept;
		NT_NODISCARD Float GetFrameTimeMs() const noexcept;
		NT_NODISCARD Float GetFrameTimeSec() const noexcept;
		NT_NODISCARD Float GetZoom() const noexcept;
		NT_NODISCARD Bool IsBindedTexture() const noexcept;
		NT_NODISCARD Bool IsInitialized() const noexcept;
		NT_NODISCARD Bool IsEnabled3D() const noexcept;

		void SetFPSLimit(const uInt& fpsLimit) noexcept;
		void SetProjection(const Matrix4x4& projection);
		void SetWorld(const Matrix4x4& world);
		void SetView(const Matrix4x4& view);
		void SetColor(const Float4D& color);
		void SetZoom(const Float& zoom) noexcept;
		void SetDepthMode(const DepthMode& mode) noexcept;
		void SetDrawingMode(const DrawingMode& mode) noexcept;
		void SetShader(Shader* pShader);

	protected:
		void _Initialize(WindowID hwnd);

	private:
		_Matrices m_Matrices;
		_Matrices m_CachedMatrices;
		_ProjectionConfig m_ProjectionConfig;
		Float4D m_Color = Colors::White;

		uIntRect m_ViewportRect;
		Shader* m_ShaderPtr = nullptr;

		ContextID m_hContext = nullptr;
		WindowID m_hwnd = nullptr;
		DescriptorID m_hdc = nullptr;
		CullFace m_CullFace = CullFace::BACK;
		DepthMode m_DepthMode = DepthMode::LESS;
		DrawingMode m_DrawingMode = DrawingMode::TRIANGLES;

		Int m_FPSLimit = 120;
		Int m_FPSCounter = 0;
		Int m_FPS = 0;
		Float m_FrameTimeMs = FLT_EPSILON;
		Timer m_LoopTimeStamp;
		Timer m_FPSTimer;

		Float m_Zoom = 1.f;
		Bool m_IsBindedTexture = false;
		Bool m_IsEnabled3D;
		Bool m_IsInitialized = false;

	private:
		void _ApplyProjectionMatrix() const;
		void _ApplyWorldMatrix() const;
		void _ApplyViewMatrix() const;
		void _ApplyRenderColor() const;
	};
}

#pragma warning(pop)