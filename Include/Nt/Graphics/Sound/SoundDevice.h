#pragma once

#include <memory>
#include <string>
#include <Nt/Core/NtDLL.h>

namespace Nt {
	NT_API std::string GetALErrorMessage();
	NT_API void CheckAlErrors(const std::string& errorMessage);

	class NT_API SoundDevice {
		struct Device;

	public:
		SoundDevice();
		~SoundDevice() noexcept;

	private:
		std::unique_ptr<Device> m_Device;
	};
}