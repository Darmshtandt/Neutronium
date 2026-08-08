#include <Nt/Graphics/Sound/Listener.h>
#include <Nt/Graphics/Sound/SoundDevice.h>
#include <AL/al.h>

namespace Nt {
	const Float3D& Listener::GetPosition() const noexcept {
		return m_Position;
	}
	const Float3D& Listener::GetRotation() const noexcept {
		return m_Rotation;
	}

	void Listener::SetPosition(const Float3D& position) {
		if (m_Position == position)
			return;

		m_Position = position;
		alListener3f(AL_POSITION, m_Position.x, m_Position.y, m_Position.z);
		CheckAlErrors("Failed to change listener position.");
	}

	void Listener::SetRotation(const Float3D& rotation) {
		if (m_Rotation == rotation)
			return;
		m_Rotation = rotation;

		Float3D up;
		const Float3D forward = Float3D(sinf(m_Rotation.x), tanf(m_Rotation.y), cosf(m_Rotation.z)).GetNormalize();

		Float3D values[2] = { up, forward };
		alListenerfv(AL_ORIENTATION, reinterpret_cast<Float*>(values));
		CheckAlErrors("Failed to change listener orientation.");
	}
}
