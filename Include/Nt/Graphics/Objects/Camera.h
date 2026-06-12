#pragma once

#include <Nt/Graphics/Objects/IObject.h>

namespace Nt {
	class Camera : public IObject {
	public:
		NT_API Camera() noexcept = default;
		NT_API Camera(const Float3D& position, const Float3D& angle) noexcept;
		Camera(const Camera&) noexcept = default;
		Camera(Camera&&) noexcept = default;
		~Camera() noexcept override = default;

		Camera& operator = (const Camera&) noexcept = default;
		Camera& operator = (Camera&&) noexcept = default;

		NT_API virtual void Render([[maybe_unused]] NotNull<Renderer*> pRenderer) const override;

		NT_API NT_NODISCARD Matrix4x4 GetView() const noexcept;

	private:
		Matrix4x4 m_View;

	private:
		using IObject::GetSize;
		using IObject::GetColor;

		using IObject::SetColor;
		using IObject::SetSize;

		NT_API virtual void Render([[maybe_unused]] NotNull<Renderer*> pRenderer, [[maybe_unused]] const uInt& offset, [[maybe_unused]] const uInt& verticesCount) const override;
	};

	class Camera2D : public Camera {
	public:
		Camera2D() noexcept = default;
		NT_API explicit Camera2D(const Float2D& position) noexcept;
		Camera2D(const Camera2D&) noexcept = default;
		Camera2D(Camera2D&&) noexcept = default;
		~Camera2D() noexcept override = default;

		Camera2D& operator = (const Camera2D&) noexcept = default;
		Camera2D& operator = (Camera2D&&) noexcept = default;

		NT_API void EnableClampPosition() noexcept;
		NT_API void DisableClampPosition() noexcept;

		NT_API void Translate(const Float2D& offset) noexcept;
		NT_API void Rotate(const Float& angle) noexcept;

		NT_API void SetMinPosition(const Float2D& minPosition) noexcept;
		NT_API void SetMaxPosition(const Float2D& maxPosition) noexcept;
		NT_API void SetPosition(const Float2D& position) noexcept;
		NT_API void SetOrigin(const Float2D& origin) noexcept;
		NT_API void SetAngle(Float angle) noexcept;

		NT_API NT_NODISCARD Float2D GetPosition() const noexcept;
		NT_API NT_NODISCARD Float2D GetOrigin() const noexcept;
		NT_API NT_NODISCARD Float2D GetMinPosition() const noexcept;
		NT_API NT_NODISCARD Float2D GetMaxPosition() const noexcept;
		NT_API NT_NODISCARD Float GetAngle() const noexcept;

	private:
		Float2D m_MinPosition;
		Float2D m_MaxPosition;
		Bool m_fClampPosition;

	private:
		using Camera::Translate;
		using Camera::Rotate;

		using Camera::GetAngle;
		using Camera::SetAngle;

		NT_API void ClampPosition() noexcept;
	};
}