#pragma once

#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Input.h>

#ifdef ERROR
#	undef ERROR
#endif

#ifdef TRANSPARENT
#	undef TRANSPARENT
#endif

#ifdef XBUTTON1
#	undef XBUTTON1
#endif

#ifdef XBUTTON2
#	undef XBUTTON2
#endif

namespace Nt {
	enum class ResizeType {
		RESTORED,
		MINIMIZED,
		MAXIMIZED,
		MAXSHOW,
		MAXHIDE
	};
	enum class ResizingType {
		NONE,
		LEFT,
		RIGHT,
		TOP,
		TOPLEFT,
		TOPRIGHT,
		BOTTOM,
		BOTTOMLEFT,
		BOTTOMRIGHT
	};
	enum class HitTest {
		ERROR = -2,
		TRANSPARENT = -1,
		NOWHERE = 0,
		CLIENT = 1,
		CAPTION = 2,
		SYSMENU = 3,
		SIZE = 4,
		GROWBOX = 4,
		MENU = 5,
		HSCROLL = 6,
		VSCROLL = 7,
		MINBUTTON = 8,
		REDUCE = 8,
		MAXBUTTON = 9,
		ZOOM = 9,
		LEFT = 10,
		RIGHT = 11,
		TOP = 12,
		TOPLEFT = 13,
		TOPRIGHT = 14,
		BOTTOM = 15,
		BOTTOMLEFT = 16,
		BOTTOMRIGHT = 17,
		BORDER = 18,
		CLOSE = 20,
		HELP = 21,
	};
	enum class MouseMode {
		LBUTTON = 0x0001,
		RBUTTON = 0x0002,
		SHIFT = 0x0004,
		CONTROL = 0x0008,
		MBUTTON = 0x0010,
		XBUTTON1 = 0x0020,
		XBUTTON2 = 0x0040
	};

	struct WindowListener {
		virtual void Create() 
		{
		}
		virtual void Resize(const ResizeType& type, const Int2D& newSize)
		{
		}
		virtual void Resizing(const ResizingType& type, IntRect& newRect)
		{
		}
		virtual void Close() 
		{
		}
	};

	struct KeyboardListener {
		virtual void KeyPressed(const Key& key)
		{
		}
		virtual void KeyReleassed(const Key& key)
		{
		}
	};

	struct MouseListener {
		virtual void ButtonPressed(const Mouse::Button& button)
		{
		}
		virtual void ButtonReleassed(const Mouse::Button& button)
		{
		}

		virtual void Move(const MouseMode& mode, const Int2D& position)
		{
		}
		virtual void Hover(const MouseMode& mode, const Int2D& position)
		{
		}
		virtual void Leave()
		{
		}

		virtual void HorizontalWheel(const MouseMode& mode, const Int& delta, const Int2D& position)
		{
		}
		virtual void Wheel(const MouseMode& mode, const Int& delta, const Int2D& position)
		{
		}

		virtual void NoClientHover(const HitTest& hitTest, const Int2D& position)
		{
		}
		virtual void NoClientLeave()
		{
		}
		virtual void NoClientMove(const HitTest& hitTest, const Int2D& position)
		{
		}
	};
}