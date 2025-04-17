// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Objects/IObject.h>
#include <Nt/Core/Colors.h>

namespace Nt {
	IObject::IObject() noexcept :
		m_Color(Colors::White),
		m_Size(1.f, 1.f, 1.f),
		m_IsChanged(false),
		m_IsRenderEnabled(true) 
	{
	}
	IObject::IObject(const IObject& Object) noexcept :
		m_Position(Object.m_Position),
		m_Origin(Object.m_Origin),
		m_Angle(Object.m_Angle),
		m_AngleOrigin(Object.m_AngleOrigin),
		m_Size(Object.m_Size),
		m_Color(Object.m_Color),
		m_IsChanged(Object.m_IsChanged),
		m_IsRenderEnabled(Object.m_IsRenderEnabled)
	{ 
	}

	void IObject::Render(Renderer* pRenderer) const
	{
	}

	void IObject::Translate(const Float3D& Offset) {
		SetPosition(m_Position + Offset);
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

	void IObject::UnmarkChanged() noexcept {
		m_IsChanged = false;
	}

	void IObject::DisableRender() noexcept {
		m_IsRenderEnabled = false;
	}
	void IObject::EnableRender() noexcept {
		m_IsRenderEnabled = true;
	}

	Matrix4x4 IObject::LocalToWorld() const noexcept {
		return
			Matrix4x4::GetTranslate(m_Position) *
			Matrix4x4::GetRotateY(m_Angle.y) *
			Matrix4x4::GetRotateX(m_Angle.x) *
			Matrix4x4::GetRotateZ(m_Angle.z) *
			Matrix4x4::GetScale(m_Size);
	}
	Matrix4x4 IObject::WorldToLocal() const noexcept {
		return
			Matrix4x4::GetScale(m_Size) *
			Matrix4x4::GetRotateZ(m_Angle.z) *
			Matrix4x4::GetRotateX(m_Angle.x) *
			Matrix4x4::GetRotateY(m_Angle.y) *
			Matrix4x4::GetTranslate(m_Position);
	}

	_CONSTEXPR23 Float3D IObject::GetPosition() const noexcept {
		return m_Position;
	}
	_CONSTEXPR23 Float3D IObject::GetSize() const noexcept {
		return m_Size;
	}
	_CONSTEXPR23 Float3D IObject::GetAngle() const noexcept {
		return m_Angle;
	}
	_CONSTEXPR23 Float3D IObject::GetAngleOrigin() const noexcept {
		return m_AngleOrigin;
	}
	_CONSTEXPR23 Float3D IObject::GetOrigin() const noexcept {
		return m_Origin;
	}
	_CONSTEXPR23 Float4D IObject::GetColor() const noexcept {
		return m_Color;
	}
	_CONSTEXPR23 Bool IObject::IsRenderEnabled() const noexcept {
		return m_IsRenderEnabled;
	}
	_CONSTEXPR23 Bool IObject::IsChanged() const noexcept {
		return m_IsChanged;
	}

	void IObject::SetPosition(const Float3D& Position) {
		if (m_Position != Position) {
			m_Position = Position;
			m_IsChanged = true;
		}
	}
	void IObject::SetSize(const Float3D& Size) {
		if (m_Size != Size) {
			m_Size = Size;
			m_IsChanged = true;
		}
	}
	void IObject::SetAngle(const Float3D& Angle) {
		if (m_Angle != Angle) {
			m_Angle = Angle;
			m_IsChanged = true;
		}
	}
	void IObject::SetAngleOrigin(const Float3D& AngleOrigin) {
		if (m_AngleOrigin != AngleOrigin) {
			m_AngleOrigin = AngleOrigin;
			m_IsChanged = true;
		}
	}
	void IObject::SetOrigin(const Float3D& Origin) {
		if (m_Origin != Origin) {
			m_Origin = Origin;
			m_IsChanged = true;
		}
	}
	void IObject::SetColor(const Float4D& Color) {
		if (m_Color != Color) {
			m_Color = Color;
			m_IsChanged = true;
		}
	}
	IObject& IObject::operator=(const IObject& object) noexcept {
		if (this == &object)
			return(*this);

		m_Position = object.m_Position;
		m_Origin = object.m_Origin;
		m_Angle = object.m_Angle;
		m_AngleOrigin = object.m_AngleOrigin;
		m_Size = object.m_Size;
		m_Color = object.m_Color;
		m_IsChanged = object.m_IsChanged;
		m_IsRenderEnabled = object.m_IsRenderEnabled;

		return(*this);
	}
}