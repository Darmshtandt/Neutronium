#pragma once

namespace Nt {
	class IResource : public ISerialization {
	public:
		enum eType {
			TYPE_NULL,
			TYPE_TEXTURE,
			TYPE_TERRAIN,
			TYPE_MESH,
			TYPE_SOUND,
			TYPE_COUNT
		};

	public:
		IResource(const uInt& Type) noexcept :
			TYPE(Type) {
			_Initialize();
		}
		virtual ~IResource() {
			SAFE_DELETE(&m_pData);
		}

		void Write(std::ostream& Stream) const override {
			Serialization::WriteAll(Stream, m_FilePath);
		}
		void Read(std::istream& Stream) override {
			Serialization::ReadAll(Stream, m_FilePath);
			if (m_FilePath.length() > 0)
				LoadFromFile(m_FilePath);
		}
		constexpr uInt ClassType() const noexcept override final {
			return TYPE;
		}
		virtual constexpr uInt Sizeof() const noexcept = 0;

		NT_API static ISerialization* New(const uInt& Type);

		void LoadFromFile(cString FilePath) {
			if (m_IsLoaded)
				Release();

			m_FilePath = FilePath;
			_LoadFromFile();

			if (!m_pData)
				Raise(L"Resource loaded but Data is nullptr");

			m_IsLoaded = true;
		}
		void Release() {
			_Release();
			m_IsLoaded = false;
			m_FilePath = "";
			SAFE_DELETE(&m_pData);
		}

		void CheckIfItsLoaded() const {
			if (!m_IsLoaded)
				Raise(L"Resource loaded but Data is nullptr");
		}


		String GetFilePath() const noexcept {
			return m_FilePath;
		}
		Bool IsLoaded() const noexcept {
			return m_IsLoaded;
		}

	public:
		const uInt TYPE;

	private:
		String m_FilePath;
		Bool m_IsLoaded;

	protected:
		void* m_pData;

		void _DeleteData() {
			SAFE_DELETE(&m_pData);
		}
		void _ThrowError(cString ErrorMsg) const {
			String ErrorMsgStr = ErrorMsg;
			ErrorMsgStr += "\n\nFile name: ";
			ErrorMsgStr += m_FilePath;
			ErrorMsgStr += "\nResoutce type: ";
			ErrorMsgStr += TYPE;
			Raise(ErrorMsgStr);
		};

	private:
		constexpr void _Initialize() noexcept {
			m_pData = nullptr;
			m_FilePath = "";
			m_IsLoaded = false;
		}

		virtual void _LoadFromFile() = 0;
		virtual void _Release() = 0;
	};
}