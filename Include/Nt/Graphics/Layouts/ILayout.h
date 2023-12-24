#pragma once

namespace Nt {
	struct ILayout {
		struct DebugGridColorConfig {
			Byte3D LayoutPaddingColor = { 195, 63, 0 };
			Byte3D CellPaddingColor = { 127, 195, 0 };
			Byte3D FrameColor = { 63, 63, 255 };
			Byte3D GapColor = { 255, 63, 255 };
		};

		virtual void UpdateContent() noexcept = 0;
		virtual void ResetLayout() = 0;

	public:
		Bool IsVisibleDebugGrid = false;
		DebugGridColorConfig DebugGridColorConfig = { };

	protected:
		void _DrawPaddingRect(HDC hdc, const IntRect& rect, const IntRect& padding, const Bool& isLayoutPadding) {
			const RECT leftSide = { 
				rect.Left, 
				rect.Top,
				rect.Left + padding.Left,
				rect.Top + rect.Bottom
			};
			const RECT topSide = {
				rect.Left, 
				rect.Top,
				rect.Left + rect.Right,
				rect.Top + padding.Top
			};
			const RECT rightSide = {
				rect.Left + rect.Right - padding.Right,
				rect.Top,
				rect.Left + rect.Right,
				rect.Top + rect.Bottom
			};
			const RECT bottomSide = {
				rect.Left,
				rect.Top + rect.Bottom - padding.Bottom,
				rect.Left + rect.Right,
				rect.Top + rect.Bottom
			};

			const HBRUSH hBrush =
				CreateSolidBrush((isLayoutPadding) ? DebugGridColorConfig.LayoutPaddingColor : DebugGridColorConfig.CellPaddingColor);

			FillRect(hdc, &leftSide, hBrush);
			FillRect(hdc, &topSide, hBrush);
			FillRect(hdc, &rightSide, hBrush);
			FillRect(hdc, &bottomSide, hBrush);
		}

		IntRect _GetGapRect(const IntRect& layerRect, const IntRect& layerPaddingRect, const Int& drawGrapPosition1D, const Int& gap, const Bool& isVerical) {
			IntRect gapRect;
			if (isVerical) {
				gapRect = {
					layerRect.Left + drawGrapPosition1D,
					layerRect.Top + layerPaddingRect.Top,
					layerRect.Left + drawGrapPosition1D + gap,
					layerRect.Top + layerRect.Bottom - layerPaddingRect.Bottom
				};
			}
			else {
				gapRect = {
					layerRect.Left + layerPaddingRect.Left,
					layerRect.Top + drawGrapPosition1D,
					layerRect.Left + layerRect.Right - layerPaddingRect.Right,
					layerRect.Top + drawGrapPosition1D + gap
				};
			}
			return gapRect;
		}
		void _DrawGapRect(HDC hdc, const IntRect& layerRect, const IntRect& layerPaddingRect, const Int& drawGrapPosition1D, const Int& gap, const Bool& isVerical) {
			const IntRect gapRect = _GetGapRect(layerRect, layerPaddingRect, drawGrapPosition1D, gap, isVerical);
			if (gapRect.Left == gapRect.Right || gapRect.Top == gapRect.Bottom)
				return;

			HPEN hPen = CreatePen(PS_SOLID, 1, VectorToColorRef(DebugGridColorConfig.GapColor));
			if (hPen == nullptr) {
				Log::Warning("Failed to create pen");
				return;
			}
			if (SelectObject(hdc, hPen) == nullptr) {
				Log::Warning("Failed to select pen");
				return;
			}

			const Int lineDistance = (layerRect.Right / 20);
			for (Int i = -layerRect.Right; i < layerRect.Right + lineDistance; i += lineDistance) {
				Float2D lineStart = {
					Float(lineDistance + i),
					Float(layerPaddingRect.Top)
				};
				Float2D lineEnd = {
					lineStart.x + Float(layerRect.Right),
					lineStart.y + Float(layerRect.Bottom - layerPaddingRect.Bottom)
				};

				if (lineStart.x >= gapRect.Right)
					continue;
				else if (lineStart.y >= gapRect.Bottom)
					continue;
				else if (lineEnd.x <= gapRect.Left)
					continue;
				else if (lineEnd.y <= gapRect.Top)
					continue;

				const Float x0 = lineStart.x;
				const Float y0 = lineStart.y;
				const Float x1 = lineEnd.x;
				const Float y1 = lineEnd.y;

				if (lineStart.x < gapRect.Left) {
					lineStart.x = gapRect.Left;
					lineStart.y = y0 + (lineStart.x - x0) * (y1 - y0) / (x1 - x0);
				}
				if (lineEnd.x > gapRect.Right) {
					lineEnd.x = gapRect.Right;
					lineEnd.y = y0 + (lineEnd.x - x0) * (y1 - y0) / (x1 - x0);
				}

				if (!isVerical) {
					lineStart.y = gapRect.Top;
					lineStart.x = x0 + (lineStart.y - y0) * (x1 - x0) / (y1 - y0);

					lineEnd.y = gapRect.Bottom;
					lineEnd.x = x0 + (lineEnd.y - y0) * (x1 - x0) / (y1 - y0);
				}

				if (lineStart.x < gapRect.Left)
					continue;
				else if (lineStart.y < gapRect.Top)
					continue;
				else if (lineEnd.x > gapRect.Right)
					continue;
				else if (lineEnd.y > gapRect.Bottom)
					continue;

				MoveToEx(hdc, (Int)lineStart.x, (Int)lineStart.y, nullptr);
				LineTo(hdc, (Int)lineEnd.x, (Int)lineEnd.y);
			}

			DeleteObject(hPen);
			SelectObject(hdc, nullptr);

			DrawFrameRect(hdc, IntRect(gapRect.LeftTop, gapRect.RightBottom - gapRect.LeftTop), 1, DebugGridColorConfig.GapColor);
		}
	};
}