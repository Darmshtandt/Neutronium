#pragma once

#include <fstream>

#include <Nt/Core/Math/Vector.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Graphics/Resources/IResource.h>

namespace Nt {
	class Image : public IResource {
	public:
		NT_API Image() noexcept = default;
		NT_API Image(const String& filePath);
		NT_API Image(const Image& otherImage);
		NT_API Image(Image&& otherImage) noexcept;
		NT_API ~Image() override;

		NT_API void LoadFromFile(const Nt::String& filePath) override;
		NT_API void Release() override;

		NT_API void Write(std::ostream& stream) const override;
		NT_API void Read(std::istream& Stream) override;

		_NODISCARD _CONSTEXPR20 uInt Sizeof() const noexcept override {
			return sizeof(*this);
		}

		NT_API _NODISCARD String GetFilePath() const noexcept override;
		NT_API _NODISCARD std::type_index GetType() const;

		NT_API _NODISCARD const std::unique_ptr<Byte[]>& GetData() const noexcept;
		NT_API _NODISCARD std::unique_ptr<Byte[]>& GetData() noexcept;
		NT_API _NODISCARD uInt2D GetSize() const noexcept;
		NT_API _NODISCARD uInt GetBitCount() const noexcept;
		NT_API _NODISCARD uInt GetChannelCount() const noexcept;

		NT_API void SetData(std::unique_ptr<Byte[]>&& pData, const uInt2D& size, const uInt& bitCount) noexcept;

	private:
		std::string m_FilePath;
		uInt2D m_Size;
		uInt m_BitCount = 0;
		std::unique_ptr<Byte[]> m_pData;
	};
}