// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Graphics/Objects/Camera.h>
#include <algorithm>

namespace Nt {
	Camera::Camera(const Float3D& Position, const Float3D& Angle) noexcept {
		SetPosition(Position);
		SetAngle(Angle);
	}

	Matrix4x4 Camera::GetView() const noexcept {
		return WorldToLocal();
	}

	void Camera::Render([[maybe_unused]] NotNull<Renderer*> pRenderer) const {
	}
	void Camera::Render([[maybe_unused]] NotNull<Renderer*> pRenderer, [[maybe_unused]] const uInt& offset, [[maybe_unused]] const uInt& verticesCount) const {
	}


	Camera2D::Camera2D(const Float2D& Position) noexcept :
		Camera(Float3D(Position, 0.f), { }),
		m_fClampPosition(false) 
	{
	}

	void Camera2D::EnableClampPosition() noexcept {
		m_fClampPosition = true;
		ClampPosition();
	}
	void Camera2D::DisableClampPosition() noexcept {
		m_fClampPosition = false;
	}

	void Camera2D::Translate(const Float2D& Offset) noexcept {
		Camera::Translate(Float3D(Offset, 0.f));
		ClampPosition();
	}
	void Camera2D::Rotate(const float& Angle) noexcept {
		Camera::Rotate({ 0.f, 0.f, Angle });
	}

	void Camera2D::SetMinPosition(const Float2D& MinPosition) noexcept {
		m_MinPosition = MinPosition;
		ClampPosition();
	}
	void Camera2D::SetMaxPosition(const Float2D& MaxPosition) noexcept {
		m_MaxPosition = MaxPosition;
		ClampPosition();
	}
	void Camera2D::SetPosition(const Float2D& Position) noexcept {
		Camera::SetPosition(Float3D(Position, 0.f));
		ClampPosition();
	}
	void Camera2D::SetOrigin(const Float2D& Origin) noexcept {
		Camera::SetOrigin(Float3D(Origin, 0.f));
	}
	void Camera2D::SetAngle(float Angle) noexcept {
		Camera::SetAngle({ 0.f, 0.f, Angle });
	}

	Float2D Camera2D::GetPosition() const noexcept {
		return Camera::GetPosition();
	}
	Float2D Camera2D::GetOrigin() const noexcept {
		return Camera::GetOrigin();
	}
	Float2D Camera2D::GetMinPosition() const noexcept {
		return m_MinPosition;
	}
	Float2D Camera2D::GetMaxPosition() const noexcept {
		return m_MaxPosition;
	}
	Float Camera2D::GetAngle() const noexcept {
		return Camera::GetAngle().z;
	}

	void Camera2D::ClampPosition() noexcept {
		if (m_fClampPosition && IsDirty()) {
			Float2D Position = GetPosition();
			Position.x = std::clamp(Position.x, m_MinPosition.x, m_MaxPosition.x);
			Position.y = std::clamp(Position.y, m_MinPosition.y, m_MaxPosition.y);
			SetPosition(Position);
		}
	}
}