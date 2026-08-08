#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Nt/Graphics/Resources/IResource.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Nt {
	union Pixel final {
		uInt Value = 0;
		Byte Array[4];
		Byte4D Vector;
	};

	class NT_API Image : public IResource {
	public:
		Image() noexcept = default;
		Image(const String& filePath);
		Image(const Image& other);
		Image(Image&& otherImage) noexcept;
		~Image() override;

		void LoadFromFile(const String& filePath) override;
		void Release() override;

		NT_NODISCARD String GetFilePath() const noexcept override;
		NT_NODISCARD std::type_index GetType() const;

		NT_NODISCARD const Byte* GetData() const noexcept;
		NT_NODISCARD Byte* GetData() noexcept;
		NT_NODISCARD uInt2D GetSize() const noexcept;
		NT_NODISCARD uInt GetBitCount() const noexcept;
		NT_NODISCARD uInt GetChannelCount() const noexcept;
		NT_NODISCARD Bool IsValid() const noexcept;

		void SetData(Byte* pData, const uInt2D& size, const uInt& bitCount) noexcept;

	private:
		std::string m_FilePath;
		uInt2D m_Size;
		uInt m_BitCount = 0;
		std::unique_ptr<Byte[]> m_pData = nullptr;
	};
}

#pragma warning(pop)