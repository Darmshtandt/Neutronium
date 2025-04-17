#pragma once

#include <Nt/Core/Math/Matrix4x4.h>

namespace Nt {
	class Renderer;

	class IObject {
	public:
		NT_API IObject() noexcept;
		NT_API IObject(const IObject& Object) noexcept;

		virtual void Render(Renderer* pRenderer) const = 0;
		virtual void Render(Renderer* pRenderer, const uInt& offset, const uInt& verticesCount) const = 0;

		NT_API void Translate(const Float3D& Offset);
		NT_API void Scale(const Float3D& scale);
		NT_API void Rotate(const Float3D& Angle);
		NT_API void RotateAroundOrigin(const Float3D& AngleOrigin);

		NT_API void UnmarkChanged() noexcept;

		NT_API void DisableRender() noexcept;
		NT_API void EnableRender() noexcept;

		NT_API _NODISCARD Matrix4x4 LocalToWorld() const noexcept;
		NT_API _NODISCARD Matrix4x4 WorldToLocal() const noexcept;

		NT_API _NODISCARD _CONSTEXPR20 Float3D GetPosition() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float3D GetSize() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float3D GetAngle() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float3D GetAngleOrigin() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float3D GetOrigin() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Float4D GetColor() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Bool IsRenderEnabled() const noexcept;
		NT_API _NODISCARD _CONSTEXPR20 Bool IsChanged() const noexcept;

		NT_API virtual void SetPosition(const Float3D& Position);
		NT_API virtual void SetSize(const Float3D& Size);
		NT_API virtual void SetAngle(const Float3D& Angle);
		NT_API virtual void SetAngleOrigin(const Float3D& AngleOrigin);
		NT_API virtual void SetOrigin(const Float3D& Origin);
		NT_API virtual void SetColor(const Float4D& Color);

		NT_API IObject& operator = (const IObject& object) noexcept;

	protected:
		Float3D m_Position;
		Float3D m_Origin;
		Float3D m_Angle;
		Float3D m_AngleOrigin;
		Float3D m_Size;
		Float4D m_Color;

	private:
		Bool m_IsChanged;
		Bool m_IsRenderEnabled;
	};
}