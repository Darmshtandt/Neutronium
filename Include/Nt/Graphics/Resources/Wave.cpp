#include <Nt/Graphics/Resources/Wave.h>
#include <Nt/Core/Log.h>
#include <fstream>

namespace Nt {
	Wave::Wave(const String& filePath) {
		LoadFromFile(filePath);
	}

	Wave::Wave(const Wave& wave) :
		m_Header(wave.m_Header),
		m_FilePath(wave.m_FilePath),
		m_SampleSize(wave.m_SampleSize),
		m_SampleCount(wave.m_SampleCount),
		m_Size(wave.m_Size),
		m_IsLoaded(wave.m_IsLoaded) 
	{
		if (m_IsLoaded && m_Size > 0) {
			m_pData = new Byte[m_Size];
			memcpy(m_pData, wave.m_pData, m_Size);
		}
	}

	Wave::~Wave() {
		Release();
	}

	void Wave::LoadFromFile(const String& filePath) {
		if (m_IsLoaded) {
			Log::Instance().Warning("File \"" + Nt::String(filePath) + "\" already loaded.");
			return;
		}

		m_FilePath = filePath;

		std::ifstream file(m_FilePath, std::ios::in | std::ios::binary);

		if (!file.is_open())
			Raise(String("Failed to open file: ") + m_FilePath);

		file.seekg(0, std::ifstream::_Seekbeg);
		file.read(reinterpret_cast<Char*>(&m_Header), sizeof(Header));

		if (memcmp(m_Header.Chunk.ID, "RIFF", 4) != 0)
			Raise("Header chunk not is RIFF");

		if (memcmp(m_Header.Format, "WAVE", 4) != 0)
			Raise("Header format not is WAVE");

		if (memcmp(m_Header.Subchunk.ID, "fmt ", 4) != 0)
			Raise("Header subchunk not is fmt");


		if (m_Header.BitsPerSample != 8 && m_Header.BitsPerSample != 16)
			Raise("Wrong number of bit per sample");

		Header::_Chunk chunk;

		do {
			file.seekg(chunk.Size, std::ifstream::_Seekcur);
			file.read(reinterpret_cast<Char*>(&chunk), sizeof(chunk));
		}
		while (memcmp(chunk.ID, "data", 4) != 0);

		m_Size = chunk.Size;
		m_SampleSize = m_Header.BitsPerSample / 8;
		m_SampleCount = m_Size / (m_Header.NumChannels * m_Header.BitsPerSample / 8);

		m_pData = new Byte[m_Size];
		file.read(reinterpret_cast<Char*>(m_pData), m_Size);

		m_IsLoaded = true;
	}

	void Wave::Release() {
		if (m_IsLoaded) {
			SAFE_DELETE(&m_pData);
			m_IsLoaded = false;
		}
	}

	_NODISCARD Wave::Header Wave::GetHeader() const noexcept {
		return m_Header;
	}

	_NODISCARD void* Wave::GetData() const noexcept {
		return m_pData;
	}

	_NODISCARD uInt Wave::GetSampleCount() const noexcept {
		return m_SampleCount;
	}

	_NODISCARD uInt Wave::GetSize() const noexcept {
		return m_Size;
	}
}