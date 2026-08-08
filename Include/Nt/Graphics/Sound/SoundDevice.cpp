#include <Nt/Graphics/Sound/SoundDevice.h>
#include <Nt/Core/Utilities.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace Nt {
	struct SoundDevice::Device final {
		ALCdevice* m_pAlDevice = nullptr;
		ALCcontext* m_pAlContext = nullptr;
	};

	std::string GetALErrorMessage() {
		const Int errorCode = alGetError();
		if (errorCode == AL_NO_ERROR)
			return "";

		switch (errorCode) {
		case AL_INVALID_NAME:
			return "OpenAL error: Invalid name.";

		case AL_INVALID_ENUM:
			return "OpenAL error: Invalid enum.";

		case AL_INVALID_VALUE:
			return "OpenAL error: Invalid value.";

		case AL_INVALID_OPERATION:
			return "OpenAL error: Invalid operation.";

		case AL_OUT_OF_MEMORY:
			return "OpenAL error: Out of memory.";
		}

		return "OpenAL error code: " + std::to_string(errorCode);
	}

	void CheckAlErrors(const std::string& errorMessage) {
		const std::string openALMessage = GetALErrorMessage();
		if (!openALMessage.empty())
			Raise(errorMessage + '\n' + openALMessage);
	}

	SoundDevice::SoundDevice() : m_Device(new Device) {
		try {
			m_Device->m_pAlDevice = alcOpenDevice(nullptr);
			RequireNotNull(m_Device->m_pAlDevice, "Failed to open audio device.");

			m_Device->m_pAlContext = alcCreateContext(m_Device->m_pAlDevice, nullptr);
			RequireNotNull(m_Device->m_pAlContext, "Failed to create audio context.");

			if (!alcMakeContextCurrent(m_Device->m_pAlContext))
				Raise("Failed to make current audio context.");
		}
		catch (const Error& error) {
			const String message = String(error.what()) + "\n" + GetALErrorMessage();
			throw Error(message, error.Caption, error.FileName, error.Line, error.FunctionName, error.Stacktrace);
		}

		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
		CheckAlErrors("Failed to change distance model.");

		alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
		CheckAlErrors("Failed to change listener position.");

		alListener3f(AL_VELOCITY, 1.f, 1.f, 1.f);
		CheckAlErrors("Failed to change listener velocity.");

		alListenerf(AL_GAIN, 0.5f);
		CheckAlErrors("Failed to change listener gain.");
	}

	SoundDevice::~SoundDevice() noexcept {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_Device->m_pAlContext);
		alcCloseDevice(m_Device->m_pAlDevice);
	}
}