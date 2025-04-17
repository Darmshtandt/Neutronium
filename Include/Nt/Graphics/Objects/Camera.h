#pragma once

#include <Nt/Graphics/Objects/IObject.h>

namespace Nt {
	class Camera : public IObject {
	public:
		NT_API Camera() noexcept = default;
		NT_API Camera(const Float3D& Position, const Float3D& Angle) noexcept;

		NT_API void Update() noexcept;

		NT_API _NODISCARD Matrix4x4 GetView() const noexcept;

	private:
		Matrix4x4 m_View;

	private:
		using IObject::UnmarkChanged;

		using IObject::GetSize;
		using IObject::GetColor;

		using IObject::SetColor;
		using IObject::SetSize;

		NT_API virtual void Render([[maybe_unused]] Renderer* pRenderer) const override;
		NT_API virtual void Render([[maybe_unused]] Renderer* pRenderer, [[maybe_unused]] const uInt& offset, [[maybe_unused]] const uInt& verticesCount) const override;
	};

	class Camera2D : public Camera {
	public:
		Camera2D() noexcept = default;
		NT_API Camera2D(const Float2D& Position) noexcept;

		NT_API void EnableClampPosition() noexcept;
		NT_API void DisableClampPosition() noexcept;

		NT_API void Translate(const Float2D& Offset) noexcept;
		NT_API void Rotate(const float& Angle) noexcept;

		NT_API void SetMinPosition(const Float2D& MinPosition) noexcept;
		NT_API void SetMaxPosition(const Float2D& MaxPosition) noexcept;
		NT_API void SetPosition(const Float2D& Position) noexcept;
		NT_API void SetOrigin(const Float2D& Origin) noexcept;
		NT_API void SetAngle(float Angle) noexcept;

		NT_API _NODISCARD Float2D GetPosition() const noexcept;
		NT_API _NODISCARD Float2D GetOrigin() const noexcept;
		NT_API _NODISCARD Float2D GetMinPosition() const noexcept;
		NT_API _NODISCARD Float2D GetMaxPosition() const noexcept;
		NT_API _NODISCARD Float GetAngle() const noexcept;

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