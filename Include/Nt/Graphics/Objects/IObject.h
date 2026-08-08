#pragma once

#include <Nt/Core/Math/Matrix4x4.h>
#include <Nt/Core/Colors.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Nt {
	class Renderer;

	class NT_API IObject {
	public:
		IObject() noexcept = default;
		IObject(const IObject& object) noexcept = default;
		IObject(IObject&& object) noexcept = default;
		virtual ~IObject() noexcept = default;

		virtual void StaticUpdate();
		virtual void Render(NotNull<Renderer*> pRenderer) const = 0;
		virtual void Render(NotNull<Renderer*> pRenderer, const uInt& offset, const uInt& verticesCount) const = 0;

		void Translate(const Float3D& offset);
		void Scale(const Float3D& scale);
		void Rotate(const Float3D& Angle);
		void RotateAroundOrigin(const Float3D& angleOrigin);

		void Hide() noexcept;
		void Show() noexcept;

		NT_NODISCARD Matrix4x4 LocalToWorld() const noexcept;
		NT_NODISCARD Matrix4x4 WorldToLocal() const noexcept;

		NT_NODISCARD Float3D GetPosition() const noexcept;
		NT_NODISCARD Float3D GetSize() const noexcept;
		NT_NODISCARD Float3D GetAngle() const noexcept;
		NT_NODISCARD Float3D GetAngleOrigin() const noexcept;
		NT_NODISCARD Float3D GetOrigin() const noexcept;
		NT_NODISCARD Float4D GetColor() const noexcept;
		NT_NODISCARD Bool IsVisible() const noexcept;
		NT_NODISCARD Bool IsDirty() const noexcept;

		void SetPosition(const Float3D& position) noexcept;
		void SetSize(const Float3D& size) noexcept;
		void SetAngle(const Float3D& angle) noexcept;
		void SetAngleOrigin(const Float3D& angleOrigin) noexcept;
		void SetOrigin(const Float3D& origin) noexcept;
		void SetColor(const Float4D& color) noexcept;

		IObject& operator = (const IObject& object) noexcept = default;
		IObject& operator = (IObject&& object) noexcept = default;

	protected:
		Float3D m_Position;
		Float3D m_Origin;
		Float3D m_Angle;
		Float3D m_AngleOrigin;
		Float3D m_Size = Float3D(1.f, 1.f, 1.f);
		Float4D m_Color = Colors::White;

	private:
		mutable Matrix4x4 m_LocalToWorld = this->_ComputeLocalToWorld();
		mutable Matrix4x4 m_WorldToLocal = this->_ComputeWorldToLocal();
		mutable Bool m_IsDirty = false;
		Bool m_IsVisible = true;

	private:
		NT_NODISCARD Matrix4x4 _ComputeLocalToWorld() const noexcept;
		NT_NODISCARD Matrix4x4 _ComputeWorldToLocal() const noexcept;
		void _UpdateMatrices() const noexcept;
	};
}

#pragma warning(pop)