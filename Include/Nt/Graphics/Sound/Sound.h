#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Nt/Graphics/Resources/IResource.h>

#include <Nt/Graphics/Resources/Wave.h>

namespace Nt {
	class NT_API Sound : public IResource {
	public:
		Sound() = default;
		Sound(const Sound& sound);
		~Sound() override;

		void LoadFromFile(const String& filePath) override;

		void Release() override;

		void Play();
		void Stop();

		void ToggleLooping();
		void ToggleLooping(const Bool& isLoop);

		NT_NODISCARD std::type_index GetType() const;
		NT_NODISCARD String GetFilePath() const noexcept override;

		NT_NODISCARD Float3D GetPosition() const noexcept;
		NT_NODISCARD Float GetRolloffFactor() const noexcept;
		NT_NODISCARD Float GetReferenceDistance() const noexcept;
		NT_NODISCARD Float GetMaxDistance() const noexcept;
		NT_NODISCARD Float GetGain() const noexcept;
		NT_NODISCARD Bool IsPlaying() const noexcept;
		NT_NODISCARD Bool IsLooping() const noexcept;

		void SetRolloffFactor(const Float& factor);
		void SetReferenceDistance(const Float& distance);
		void SetMaxDistance(const Float& distance);
		void SetGain(const Float& gain);
		void SetPosition(const Float3D& position);

	private:
		std::string m_FilePath;
		Wave m_Wave;
		Float3D m_Position;

		Float m_Gain = 1.f;
		Float m_RolloffFactor = 1.f;
		Float m_ReferenceDistance = 0.f;
		Float m_MaxDistance = 0.f;

		uInt m_BufferID = 0;
		uInt m_SourceID = 0;

		Bool m_IsPlaying = false;
		Bool m_IsLooping = false;
	};
}