#include <Nt/Graphics/Sound/Sound.h>
#include <Nt/Graphics/Sound/SoundDevice.h>

#include <AL/al.h>

namespace Nt {
	Sound::Sound(const Sound& sound) :
		IResource(sound),
		m_BufferID(sound.m_BufferID),
		m_SourceID(sound.m_SourceID),
		m_Position(sound.m_Position),
		m_Gain(sound.m_Gain),
		m_RolloffFactor(sound.m_RolloffFactor),
		m_ReferenceDistance(sound.m_ReferenceDistance),
		m_MaxDistance(sound.m_MaxDistance),
		m_IsPlaying(sound.m_IsPlaying),
		m_IsLooping(sound.m_IsLooping) 
	{
	}

	Sound::~Sound() {
		Stop();
		alDeleteSources(1, &m_SourceID);
	}

	void Sound::LoadFromFile(const String& filePath) {
		m_FilePath = filePath;
		if (m_BufferID != 0) {
			if (m_SourceID != 0) {
				alSourcei(m_SourceID, AL_BUFFER, 0);
				CheckAlErrors("Failed to set buffer in source.");

				alDeleteSources(1, &m_SourceID);
				CheckAlErrors("Failed to delete source.");
			}

			alDeleteBuffers(1, &m_BufferID);
			CheckAlErrors("Failed to delete buffer.");
		}

		m_Wave.LoadFromFile(GetFilePath());

		const Bool isStereo = (m_Wave.GetHeader().NumChannels > 1);

		ALenum format = -1;
		switch (m_Wave.GetHeader().BitsPerSample) {
		case 8:
			format = (isStereo) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			break;

		case 16:
			format = (isStereo) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			break;
		}

		alGenBuffers(1, &m_BufferID);
		CheckAlErrors("Failed to create buffer.");

		alBufferData(m_BufferID, format, m_Wave.GetData(), m_Wave.GetSize(), m_Wave.GetHeader().SampleRate);
		CheckAlErrors("Failed to set data in buffer.");

		alGenSources(1, &m_SourceID);
		CheckAlErrors("Failed to create source.");

		alSource3f(m_SourceID, AL_DIRECTION, 0.f, 1.f, 0.f);
		CheckAlErrors("Failed to create source.");

		alSourcei(m_SourceID, AL_BUFFER, m_BufferID);
		CheckAlErrors("Failed to set buffer in source.");

		if (m_IsLooping)
			alSourcei(m_SourceID, AL_LOOPING, m_IsLooping);
	}

	void Sound::Release() {
		m_Wave.Release();
	}

	void Sound::Play() {
		if (m_SourceID == 0)
			return;

		if (!m_IsPlaying) {
			alSourcePlay(m_SourceID);
			CheckAlErrors("Failed to play sound.");

			m_IsPlaying = true;
		}
	}

	void Sound::Stop() {
		if (m_SourceID == 0)
			return;

		alSourcePause(m_SourceID);
		CheckAlErrors("Failed to stop sound.");

		alSourceRewind(m_SourceID);
		CheckAlErrors("Failed to rewind sound.");

		m_IsPlaying = false;
	}

	void Sound::ToggleLooping() {
		ToggleLooping(!m_IsLooping);
	}

	void Sound::ToggleLooping(const Bool& isLoop) {
		if (m_SourceID == 0)
			return;

		alSourcei(m_SourceID, AL_LOOPING, isLoop);
		CheckAlErrors("Failed to change position.");

		m_IsLooping = isLoop;
	}

	_NODISCARD std::type_index Sound::GetType() const {
		return typeid(Sound);
	}

	_NODISCARD String Sound::GetFilePath() const noexcept {
		return m_FilePath;
	}

	_NODISCARD Float3D Sound::GetPosition() const noexcept {
		return m_Position;
	}

	_NODISCARD Float Sound::GetRolloffFactor() const noexcept {
		return m_RolloffFactor;
	}

	_NODISCARD Float Sound::GetReferenceDistance() const noexcept {
		return m_ReferenceDistance;
	}

	_NODISCARD Float Sound::GetMaxDistance() const noexcept {
		return m_MaxDistance;
	}

	_NODISCARD Float Sound::GetGain() const noexcept {
		return m_Gain;
	}

	_NODISCARD Bool Sound::IsPlaying() const noexcept {
		return m_IsPlaying;
	}

	_NODISCARD Bool Sound::IsLooping() const noexcept {
		return m_IsLooping;
	}

	void Sound::SetRolloffFactor(const Float& factor) {
		if (m_SourceID != 0) {
			m_RolloffFactor = factor;
			alSourcef(m_SourceID, AL_ROLLOFF_FACTOR, m_RolloffFactor);
			CheckAlErrors("Failed to change roll-off factor.");
		}
	}

	void Sound::SetReferenceDistance(const Float& distance) {
		if (m_SourceID != 0) {
			m_ReferenceDistance = distance;
			alSourcef(m_SourceID, AL_REFERENCE_DISTANCE, m_ReferenceDistance);
			CheckAlErrors("Failed to change reference distance.");
		}
	}

	void Sound::SetMaxDistance(const Float& distance) {
		if (m_SourceID != 0) {
			m_MaxDistance = distance;
			alSourcef(m_SourceID, AL_MAX_DISTANCE, m_MaxDistance);
			CheckAlErrors("Failed to change max distance.");
		}
	}

	void Sound::SetGain(const Float& gain) {
		if (m_SourceID != 0) {
			m_Gain = gain;
			alSourcef(m_SourceID, AL_GAIN, m_Gain);
			CheckAlErrors("Failed to change gain.");
		}
	}

	void Sound::SetPosition(const Float3D& position) {
		if (m_SourceID != 0) {
			m_Position = position;
			alSourcefv(m_SourceID, AL_POSITION, reinterpret_cast<Float*>(&m_Position));
			CheckAlErrors("Failed to change position.");
		}
	}
}
