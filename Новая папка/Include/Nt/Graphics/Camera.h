#pragma once

namespace Nt {
	class Camera : public IObject {
	public:
		Camera() noexcept = default;
		Camera(const Float3D& Position, const Float3D& Angle) noexcept {
			SetPosition(Position);
			SetAngle(Angle);
		}

		void Update() noexcept {
			if (IsChanged()) {
				m_View.MakeIdentity();
				m_View.Rotate(GetAngle());
				m_View.Translate(GetOrigin());
				m_View.Rotate(GetAngleOrigin());
				m_View.Translate(GetPosition());
			}
		}

		Matrix4x4 GetView() const {
			return m_View;
		}

	private:
		Matrix4x4 m_View;

	private:
		using IObject::UnmarkChanged;

		using IObject::GetSize;
		using IObject::GetColor;

		using IObject::SetColor;
		using IObject::SetSize;

		virtual void Render(Renderer* pRenderer) const override {
		}
	};

	class Camera2D : public Camera {
	public:
		Camera2D() noexcept = default;
		Camera2D(const Float2D& Position) noexcept :
			Camera(Position, { }),
			m_fClampPosition(false) {
		}

		void EnableClampPosition() noexcept {
			m_fClampPosition = true;
			ClampPosition();
		}
		void DisableClampPosition() noexcept {
			m_fClampPosition = false;
		}

		void Translate(const Float2D& Offset) noexcept {
			Camera::Translate(Offset);
			ClampPosition();
		}
		void Rotate(const float& Angle) noexcept {
			Camera::Rotate({ 0.f, 0.f, Angle });
		}

		void SetMinPosition(const Float2D& MinPosition) noexcept {
			m_MinPosition = MinPosition;
			ClampPosition();
		}
		void SetMaxPosition(const Float2D& MaxPosition) noexcept {
			m_MaxPosition = MaxPosition;
			ClampPosition();
		}
		void SetPosition(const Float2D& Position) noexcept {
			Camera::SetPosition(Position);
			ClampPosition();
		}
		void SetOrigin(const Float2D& Origin) noexcept {
			Camera::SetOrigin(Origin);
		}
		void SetAngle(float Angle) noexcept {
			Camera::SetAngle({ 0.f, 0.f, Angle });
		}

		Float2D GetPosition() const noexcept {
			return Camera::GetPosition();
		}
		Float2D GetOrigin() const noexcept {
			return Camera::GetOrigin();
		}
		Float2D GetMinPosition() const noexcept {
			return m_MinPosition;
		}
		Float2D GetMaxPosition() const noexcept {
			return m_MaxPosition;
		}
		float GetAngle() const noexcept {
			return Camera::GetAngle().z;
		}

	private:
		Float2D m_MinPosition;
		Float2D m_MaxPosition;
		bool m_fClampPosition;

	private:
		using Camera::Translate;
		using Camera::Rotate;

		using Camera::GetAngle;
		using Camera::SetAngle;

		void ClampPosition() noexcept {
			if (m_fClampPosition) {
				Float2D Position = GetPosition();
				Position.x = std::clamp(Position.x, m_MinPosition.x, m_MaxPosition.x);
				Position.y = std::clamp(Position.y, m_MinPosition.y, m_MaxPosition.y);
				SetPosition(Position);
			}
		}
	};
}