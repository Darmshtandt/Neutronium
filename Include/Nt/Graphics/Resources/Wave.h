#pragma once

#include <Nt/Core/Utilities.h>

namespace Nt {
	class NT_API Wave {
	private:
		struct Header final {
			struct _Chunk  final {
				Char ID[4] = { };
				uInt Size = 0;
			};

			_Chunk Chunk = { };
			Char Format[4] = { };

			_Chunk Subchunk = { };
			Short AudioFormat = 0;
			Short NumChannels = 0;
			Int SampleRate = 0;
			Int ByteRate = 0;
			Short BlockAlign = 0;
			Short BitsPerSample = 0;
		};

	public:
		Wave() noexcept = default;
		explicit Wave(const String& filePath);
		Wave(const Wave& wave);
		~Wave();

		void LoadFromFile(const String& filePath);

		void Release();

		NT_NODISCARD Header GetHeader() const noexcept;
		NT_NODISCARD void* GetData() const noexcept;
		NT_NODISCARD uInt GetSampleCount() const noexcept;
		NT_NODISCARD uInt GetSize() const noexcept;

	private:
		Header m_Header = { };
		std::string m_FilePath;

		uInt m_SampleSize = 0;
		uInt m_SampleCount = 0;
		uInt m_Size = 0;

		Byte* m_pData = nullptr;
		Bool m_IsLoaded = false;
	};
}