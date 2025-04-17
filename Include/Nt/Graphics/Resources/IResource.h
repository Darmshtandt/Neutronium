#pragma once

#include <typeindex>

#include <Nt/Core/Serialization.h>

namespace Nt {
	class IResource : public ISerialization {
	public:
		IResource() noexcept = default;
		virtual ~IResource() = default;

		void Write(std::ostream& stream) const override {
			Serialization::WriteAll(stream, GetFilePath());
		}

		_NODISCARD virtual constexpr uInt Sizeof() const noexcept = 0;

		_NODISCARD 
		uInt ClassType() const noexcept override {
			return GetType().hash_code();
		}

		NT_API static ISerialization* New(const uInt& type);

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
	};
}