#pragma once

#include <Nt/Core/Utilities.h>

#ifdef ERROR
#	undef ERROR
#endif

#ifdef NONE
#	undef NONE
#endif

namespace Nt {
	enum class MessageIcon : uLong {
		NONE = 0,
		WARNING = 0x00000030L,
		ERROR = 0x00000010L,
		INFO = 0x00000040L,
		CONFIRM = 0x00000004L,
	};

	class MessageWindow {
	public:
		NT_API MessageWindow(const String& message, const String& caption);

		NT_API Int Show(const MessageIcon& types) const noexcept;

		NT_API Bool ShowConfirmed(const MessageIcon& types = MessageIcon::NONE) const noexcept;

		NT_API Int ShowError(const MessageIcon& types = MessageIcon::NONE) const noexcept;
		NT_API Int ShowWarning(const MessageIcon& types = MessageIcon::NONE) const noexcept;
		NT_API Int ShowInfo(const MessageIcon& types = MessageIcon::NONE) const noexcept;

	private:
		std::wstring m_Message;
		std::wstring m_Caption;
	};
}