#pragma once

namespace Nt {
	class Renderer;

	class IObject {
	public:
		IObject() noexcept :
			m_Color(Colors::White),
			m_Size(1.f, 1.f, 1.f),
			m_IsChanged(false),
			m_IsVisible(true) 
		{
		}
		IObject(const IObject& Object) noexcept :
			m_Position(Object.m_Position),
			m_Origin(Object.m_Origin),
			m_Angle(Object.m_Angle),
			m_AngleOrigin(Object.m_AngleOrigin),
			m_Size(Object.m_Size),
			m_Color(Object.m_Color),
			m_IsChanged(Object.m_IsChanged),
			m_IsVisible(Object.m_IsVisible)
		{ 
		}

		virtual void Render(Renderer* pRenderer) const = 0;

		void Translate(const Float3D& Offset) {
			SetPosition(m_Position + Offset);
		}
		void Resize(const Float3D& Size) {
			SetSize(m_Size + Size);
		}
		void Rotate(const Float3D& Angle) {
			SetAngle(m_Angle + Angle);
		}
		void RotateAroundOrigin(const Float3D& AngleOrigin) {
			SetAngleOrigin(m_AngleOrigin + AngleOrigin);
		}

		void UnmarkChanged() noexcept {
			m_IsChanged = false;
		}

		void Hide() noexcept {
			m_IsVisible = false;
		}
		void Show() noexcept {
			m_IsVisible = true;
		}

		Matrix4x4 LocalToWorld() const noexcept {
			return
				Matrix4x4::GetTranslate(m_Position) *
				Matrix4x4::GetRotateY(m_Angle.y) *
				Matrix4x4::GetRotateX(m_Angle.x) *
				Matrix4x4::GetRotateZ(m_Angle.z) *
				Matrix4x4::GetScale(m_Size);
		}
		Matrix4x4 WorldToLocal() const noexcept {
			return
				Matrix4x4::GetScale(m_Size) *
				Matrix4x4::GetRotateZ(m_Angle.z) *
				Matrix4x4::GetRotateX(m_Angle.x) *
				Matrix4x4::GetRotateY(m_Angle.y) *
				Matrix4x4::GetTranslate(m_Position);
		}

		Float3D GetPosition() const noexcept {
			return m_Position;
		}
		Float3D GetSize() const noexcept {
			return m_Size;
		}
		Float3D GetAngle() const noexcept {
			return m_Angle;
		}
		Float3D GetAngleOrigin() const noexcept {
			return m_AngleOrigin;
		}
		Float3D GetOrigin() const noexcept {
			return m_Origin;
		}
		Float4D GetColor() const noexcept {
			return m_Color;
		}
		Bool IsChanged() const noexcept {
			return m_IsChanged;
		}
		Bool IsVisible() const noexcept {
			return m_IsVisible;
		}

		virtual void SetPosition(const Float3D& Position) {
			if (m_Position != Position) {
				m_Position = Position;
				m_IsChanged = true;
			}
		}
		virtual void SetSize(const Float3D& Size) {
			if (m_Size != Size) {
				m_Size = Size;
				m_IsChanged = true;
			}
		}
		virtual void SetAngle(const Float3D& Angle) {
			if (m_Angle != Angle) {
				m_Angle = Angle;
				m_IsChanged = true;
			}
		}
		virtual void SetAngleOrigin(const Float3D& AngleOrigin) {
			if (m_AngleOrigin != AngleOrigin) {
				m_AngleOrigin = AngleOrigin;
				m_IsChanged = true;
			}
		}
		virtual void SetOrigin(const Float3D& Origin) {
			if (m_Origin != Origin) {
				m_Origin = Origin;
				m_IsChanged = true;
			}
		}
		virtual void SetColor(const Float4D& Color) {
			if (m_Color != Color) {
				m_Color = Color;
				m_IsChanged = true;
			}
		}

	protected:
		Float3D m_Position;
		Float3D m_Origin;
		Float3D m_Angle;
		Float3D m_AngleOrigin;
		Float3D m_Size;
		Float4D m_Color;

	private:
		Bool m_IsChanged;
		Bool m_IsVisible;
	};
}