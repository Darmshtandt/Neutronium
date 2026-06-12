#pragma once

#include <typeindex>

namespace Nt {
	class NT_API IResource {
		friend class ResourceManager;

	public:
		IResource() noexcept = default;
		virtual ~IResource() noexcept = default;

		virtual void LoadFromFile(const Nt::String& filePath) = 0;
		virtual void Release() = 0;

		_NODISCARD virtual std::type_index GetType() const = 0;
		_NODISCARD virtual String GetFilePath() const noexcept = 0;

	protected:
		void _ThrowError(cString errorMsg) const {
			String errorMsgStr = errorMsg;
			errorMsgStr += "\n\nFile path: ";
			errorMsgStr += GetFilePath();
			errorMsgStr += "\nResoutce type: ";
			errorMsgStr += GetType().name();

			Raise(errorMsgStr);
		};

	private:
		uInt m_Generation = 0;
		uInt m_Index = 0;
	};
}