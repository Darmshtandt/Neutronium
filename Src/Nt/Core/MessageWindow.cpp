// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/WinMinimal.h>

#undef NOUSER
#undef NOMB

#include <Windows.h>
#include <Nt/Core/MessageWindow.h>


namespace Nt {
	MessageWindow::MessageWindow(const String& message, const String& caption) :
		m_Message(message),
		m_Caption(caption)
	{
	}

	Int MessageWindow::Show(const MessageIcon& icon) const noexcept {
		return MessageBoxW(nullptr, m_Message.c_str(), m_Caption.c_str(), Int(icon));
	}

	Bool MessageWindow::ShowConfirmed(const MessageIcon& icon) const noexcept {
		return (Show(MessageIcon(uLong(MessageIcon::CONFIRM) | uLong(icon))) == IDYES);
	}

	Int MessageWindow::ShowError(const MessageIcon& icon) const noexcept {
		return Show(MessageIcon(uLong(MessageIcon::ERROR) | uLong(icon)));
	}
	Int MessageWindow::ShowWarning(const MessageIcon& icon) const noexcept {
		return Show(MessageIcon(uLong(MessageIcon::WARNING) | uLong(icon)));
	}
	Int MessageWindow::ShowInfo(const MessageIcon& icon) const noexcept {
		return Show(MessageIcon(uLong(MessageIcon::INFO) | uLong(icon)));
	}
}