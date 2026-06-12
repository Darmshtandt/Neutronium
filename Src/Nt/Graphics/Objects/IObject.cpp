// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Objects/IObject.h>

namespace Nt {
	void IObject::StaticUpdate() {
		if (!m_IsDirty)
			return;

		_UpdateMatrices();
		m_IsDirty = false;
	}

	void IObject::Render(NotNull<Renderer*> pRenderer) const {
		(void)pRenderer;
	}

	void IObject::Translate(const Float3D& offset) {
		SetPosition(m_Position + offset);
	}
	void IObject::Scale(const Float3D& scale) {
		SetSize(m_Size + scale);
	}
	void IObject::Rotate(const Float3D& Angle) {
		SetAngle(m_Angle + Angle);
	}
	void IObject::RotateAroundOrigin(const Float3D& AngleOrigin) {
		SetAngleOrigin(m_AngleOrigin + AngleOrigin);
	}

	void IObject::Hide() noexcept {
		if (m_IsVisible)
			m_IsVisible = false;
	}
	void IObject::Show() noexcept {
		if (!m_IsVisible)
			m_IsVisible = true;
	}

	Matrix4x4 IObject::LocalToWorld() const noexcept {
		if (m_IsDirty) {
			_UpdateMatrices();
			m_IsDirty = false;
		}
		return m_LocalToWorld;
	}
	Matrix4x4 IObject::WorldToLocal() const noexcept {
		if (m_IsDirty) {
			_UpdateMatrices();
			m_IsDirty = false;
		}
		return m_WorldToLocal;
	}

	Float3D IObject::GetPosition() const noexcept {
		return m_Position;
	}
	Float3D IObject::GetSize() const noexcept {
		return m_Size;
	}
	Float3D IObject::GetAngle() const noexcept {
		return m_Angle;
	}
	Float3D IObject::GetAngleOrigin() const noexcept {
		return m_AngleOrigin;
	}
	Float3D IObject::GetOrigin() const noexcept {
		return m_Origin;
	}
	Float4D IObject::GetColor() const noexcept {
		return m_Color;
	}
	Bool IObject::IsVisible() const noexcept {
		return m_IsVisible;
	}
	Bool IObject::IsDirty() const noexcept {
		return m_IsDirty;
	}

	void IObject::SetPosition(const Float3D& Position) noexcept {
		if (m_Position != Position) {
			m_Position = Position;
			m_IsDirty = true;
		}
	}
	void IObject::SetSize(const Float3D& Size) noexcept {
		if (m_Size != Size) {
			m_Size = Size;
			m_IsDirty = true;
		}
	}
	void IObject::SetAngle(const Float3D& Angle) noexcept {
		if (m_Angle != Angle) {
			m_Angle = Angle;
			m_IsDirty = true;
		}
	}
	void IObject::SetAngleOrigin(const Float3D& AngleOrigin) noexcept {
		if (m_AngleOrigin != AngleOrigin) {
			m_AngleOrigin = AngleOrigin;
			m_IsDirty = true;
		}
	}
	void IObject::SetOrigin(const Float3D& Origin) noexcept {
		if (m_Origin != Origin) {
			m_Origin = Origin;
			m_IsDirty = true;
		}
	}
	void IObject::SetColor(const Float4D& Color) noexcept {
		if (m_Color != Color) {
			m_Color = Color;
			m_IsDirty = true;
		}
	}

	Matrix4x4 IObject::_ComputeLocalToWorld() const noexcept {
		return
			Matrix4x4::GetTranslate(m_Position) *
			Matrix4x4::GetRotateY(m_Angle.y) *
			Matrix4x4::GetRotateX(m_Angle.x) *
			Matrix4x4::GetRotateZ(m_Angle.z) *
			Matrix4x4::GetScale(m_Size);
	}

	Matrix4x4 IObject::_ComputeWorldToLocal() const noexcept {
		return
			Matrix4x4::GetScale(m_Size) *
			Matrix4x4::GetRotateZ(m_Angle.z) *
			Matrix4x4::GetRotateX(m_Angle.x) *
			Matrix4x4::GetRotateY(m_Angle.y) *
			Matrix4x4::GetTranslate(m_Position);
	}

	void IObject::_UpdateMatrices() const noexcept {
		m_LocalToWorld = _ComputeLocalToWorld();
		m_WorldToLocal = _ComputeWorldToLocal();
	}
}