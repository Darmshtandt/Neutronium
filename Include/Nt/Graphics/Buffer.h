#pragma once

#include <Nt/Core/Log.h>

namespace Nt {
	enum UsageDraw {
		USAGE_NONE = 0x0000,

		USAGE_STREAMDRAW = 0x88E0,
		USAGE_STREAMREAD = 0x88E1,
		USAGE_STREAMCOPY = 0x88E2,

		USAGE_STATICDRAW = 0x88E4,
		USAGE_STATICREAD = 0x88E5,
		USAGE_STATICCOPY = 0x88E6,

		USAGE_DYNAMICDRAW = 0x88E8,
		USAGE_DYNAMICREAD = 0x88E9,
		USAGE_DYNAMICCOPY = 0x88EA
	};

	class NT_API Buffer {
	public:
		enum Target : uShort {
			NONE = 0x0000,
			ARRAY = 0x8892,
			ATOMIC_COUNTER = 0x92C0,
			COPY_READ = 0x8F36,
			COPY_WRITE = 0x8F37,
			DISPATCH_INDIRECT = 0x90EE,
			DRAW_INDIRECT = 0x8F3F,
			ELEMENT_ARRAY = 0x8893,
			PIXEL_PACK = 0x88EB,
			PIXEL_UNPACK = 0x88EC,
			QUERY = 0x9192,
			SHADER_STORAGE = 0x90D2,
			TEXTURE = 0x8C2A,
			TRANSFORM_FEEDBACK = 0x8C8E,
			UNIFORM = 0x8A11,
		};
		enum class Access : uShort {
			READ_ONLY = 0x88B8,
			WRITE_ONLY = 0x88B9,
			READ_WRITE = 0x88BA
		};

	public:
		explicit Buffer(const Target& target);

		Buffer() = delete;
		Buffer(const Buffer& other);
		Buffer(Buffer&& other) noexcept = default;
		~Buffer() noexcept;

		void Clear() noexcept;

		void SetData(const uInt& size, void* pData, const UsageDraw& usage) noexcept;
		void SetSubData(const uInt& offset, const uInt& size, const void* pData) const;

		NT_NODISCARD void* Map(const Access& access) const noexcept;
		void Unmap() const noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;
		
		Buffer& operator = (const Buffer& other) noexcept;
		Buffer& operator = (Buffer&& other) noexcept = default;

		NT_NODISCARD UsageDraw GetUsage() const noexcept;
		NT_NODISCARD Target GetTarget() const noexcept;
		NT_NODISCARD void* GetDataPtr() const noexcept;
		NT_NODISCARD uInt GetSize() const noexcept;
		NT_NODISCARD uInt GetID() const noexcept;

	private:
		void* m_pData = nullptr;
		Target m_Target = NONE;
		UsageDraw m_Usage = USAGE_NONE;
		uInt m_Size = 0;
		uInt m_ID = 0;

	private:
		void _Create();
	};
}