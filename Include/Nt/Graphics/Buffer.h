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

	class Buffer {
	public:
		enum Target : uInt {
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
			TRANSFORM_FEEDBAC = 0x8C8E,
			UNIFORM = 0x8A11,
		};
		enum class Access : uInt {
			READ_ONLY = 0x88B8,
			WRITE_ONLY = 0x88B9,
			READ_WRITE = 0x88BA
		};

	public:
		Buffer() noexcept = default;
		NT_API ~Buffer();

		NT_API void Create(const Target& target);

		NT_API void SetData(const uInt& size, const void* pData, const UsageDraw& usage);
		NT_API void SetSubData(const uInt& offset, const uInt& size, const void* pData);

		NT_API void* Map(const Access& access) const;
		NT_API void Unmap() const;

		NT_API void Bind() const;
		NT_API void Unbind() const noexcept;

		NT_API void Delete();
		
		NT_API _NODISCARD UsageDraw GetUsage() const noexcept;
		NT_API _NODISCARD Target GetTarget() const noexcept;
		NT_API _NODISCARD const Void* GetDataPtr() const noexcept;
		NT_API _NODISCARD uInt GetDataSize() const noexcept;
		NT_API _NODISCARD uInt GetID() const noexcept;
		NT_API _NODISCARD Bool IsCreated() const noexcept;

	private:
		const Void* m_pData = nullptr;

		UsageDraw m_Usage = USAGE_NONE;
		Target m_Target = NONE;

		uInt m_DataSize = 0;
		uInt m_ID = 0;
	};
}