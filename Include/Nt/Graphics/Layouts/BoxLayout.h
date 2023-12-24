#pragma once

namespace Nt {
	class BoxLayout : public Window, public ILayout {
	public:
		BoxLayout(const uInt& layoutSize) {
			SetLayoutSize(layoutSize);
		}
		BoxLayout(const uInt& layoutSize, const IntRect& windowRect, const String& name) :
			Window(windowRect, name)
		{
			m_SavedClientRect = GetClientRect();
			SetLayoutSize(layoutSize);
		}
		BoxLayout(const uInt& layoutSize, const Int2D& windowSize, const String& name) :
			Window(windowSize, name)
		{
			m_SavedClientRect = GetClientRect();
			SetLayoutSize(layoutSize);
		}
		BoxLayout(const uInt& layoutSize, const String& name) :
			Window(name)
		{
			m_SavedClientRect = GetClientRect();
			SetLayoutSize(layoutSize);
		}
		
		void Create(const String& Name) override {
			constexpr IntRect defaultWindowRect = 
				{ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };

			if (m_WindowRect.Right == 0 || m_WindowRect.Bottom == 0)
				Create(defaultWindowRect, Name);
			else
				Create(m_WindowRect, Name);
		}
		void Create(const Int2D& size, const String& name) override {
			if (IsCreated()) {
				Log::Warning("The handle has already been created");
				return;
			}

			Window::Create(size, name);

			m_SavedClientRect = GetClientRect();
			ResetLayout();
		}
		void Create(const IntRect& windowRect, const String& name) override {
			if (IsCreated()) {
				Log::Warning("The handle has already been created");
				return;
			}

			Window::Create(windowRect, name);

			m_SavedClientRect = GetClientRect();
			ResetLayout();
		}

		void UpdateContent() noexcept {
			IntRect contentRect = _GetContentRect();
			Int& contentSize = (m_IsVertical) ? contentRect.Bottom : contentRect.Right;
			if (m_Gap > 0)
				contentSize -= m_Gap * Int(roundf(Float(m_LayoutSize) / 2.f));

			IntRect cellDefaultRect = contentRect;
			Int& cellDefaultPosition = (m_IsVertical) ? cellDefaultRect.Top : cellDefaultRect.Left;
			Int& cellDefaultSize = (m_IsVertical) ? cellDefaultRect.Bottom : cellDefaultRect.Right;

			Int extraPixels = contentSize % Int(m_LayoutSize);
			cellDefaultSize /= Int(m_LayoutSize);

			for (uInt i = 0; i < m_LayoutSize; ++i) {
				FloatRect newCellRect = cellDefaultRect;

				Float& newCellSize = (m_IsVertical) ? newCellRect.Bottom : newCellRect.Right;
				newCellSize = std::roundf(newCellSize * m_CellsSchematic[i]);

				if (extraPixels > 0) {
					newCellSize += 1.f;
					--extraPixels;
				}
				m_Cells[i].SetRect(newCellRect);

				cellDefaultPosition += Int(newCellSize);
				if (i % 2 == 0 && m_Gap > 0 && (i + 1) != m_LayoutSize)
					cellDefaultPosition += m_Gap;
			}
		}
		void ResetLayout() {
			if (m_SavedClientRect.Right == 0 || m_SavedClientRect.Bottom == 0)
				return;

			m_CellsSchematic.resize(m_LayoutSize, 1);
			UpdateContent();
		}

		void Insert(const uInt& cellIndex, HandleWindow* pHandle) {
			if (cellIndex >= m_Cells.size())
				Raise("Out of range");

			if (pHandle != nullptr) {
				pHandle->SetParent(*this);

				const HWND handle = pHandle->GetHandle();
				if (handle != nullptr) {
					for (const Cell& cell : m_Cells) {
						if (cell.GetHandle() != nullptr && cell.GetHandle()->GetHandle() == handle) {
							Log::Warning("This element has already been added to the BoxLayout");
							return;
						}
					}
				}
			}

			m_Cells[cellIndex].SetHandle(pHandle);
		}

		void TogleVertical(const Bool& isVertical) noexcept {
			if (m_IsVertical == isVertical)
				return;

			m_IsVertical = isVertical;

			if (m_SavedClientRect.Right == 0 || m_SavedClientRect.Bottom == 0)
				return;

			for (Cell& cell : m_Cells) {
				const DoubleRect rectPercentage = 
					DoubleRect(cell.GetRect()) / Double2D(m_SavedClientRect.RightBottom);

				DoubleRect newRect = DoubleRect(m_SavedClientRect.RightBottom, m_SavedClientRect.RightBottom);
				newRect.Left *= rectPercentage.Top;
				newRect.Top *= rectPercentage.Left;
				newRect.Right *= rectPercentage.Bottom;
				newRect.Bottom *= rectPercentage.Right;
				
				cell.SetRect(newRect);
			}
		}
		void TogleVertical() noexcept {
			TogleVertical(!m_IsVertical);
		}

		Cell& operator [] (const uInt& index) {
			if (index >= m_Cells.size())
				Raise("Out of range");
			return m_Cells[index];
		}

		Int GetCellSize(const uInt& index) const {
			if (index >= m_Cells.size())
				Raise("Out of range");

			const Int contentSize = (m_IsVertical) ? _GetContentRect().Bottom : _GetContentRect().Right;
			const Int extraPixels = contentSize % Int(m_LayoutSize);
			const Int cellSize = (m_IsVertical) ? m_Cells[index].GetRect().Bottom : m_Cells[index].GetRect().Right;

			if (index < extraPixels)
				return cellSize - 1;
			return cellSize;
		}
		FloatRect GetPercentagePaddingRect() const noexcept {
			return m_PercentagePaddingRect;
		}
		FloatRect GetPaddingRect() const noexcept {
			return m_PercentagePaddingRect * Float2D(m_WindowRect.RightBottom);
		}
		Int GetGap() const noexcept {
			return m_Gap;
		}

		Bool IsVertical() const noexcept {
			return m_IsVertical;
		}

		void SetPadding(const FloatRect& paddingRect, const UnitType& unit) {
			if (unit == Nt::UnitType::UNIT_PIXEL && (!IsCreated()))
				Raise("Layout is not created");

			FloatRect newPercentagePaddingRect = paddingRect;
			if (unit != UnitType::UNIT_PERCENTAGE)
				newPercentagePaddingRect = ConvertUnit(paddingRect, unit, UnitType::UNIT_PERCENTAGE, m_ClientRect.RightBottom);

			if (m_PercentagePaddingRect != newPercentagePaddingRect) {
				m_PercentagePaddingRect = newPercentagePaddingRect;
				UpdateContent();
			}
		}
		void SetLayoutSize(const uInt& newLayoutSize) {
			if (m_LayoutSize != newLayoutSize) {
				m_LayoutSize = newLayoutSize;

				m_Cells.resize(m_LayoutSize);
				if (m_LayoutSize > 0)
					ResetLayout();
			}
		}
		void SetGap(Int newGap, const UnitType& unit) {
			if (unit != UnitType::UNIT_PIXEL) {
				newGap = ConvertUnit(
					newGap, unit, UnitType::UNIT_PIXEL, ((m_IsVertical) ? m_ClientRect.Bottom : m_ClientRect.Right));
			}

			if (m_Gap == newGap)
				return;

			const Int gapInfelicity = (newGap - m_Gap);

			IntRect infelicityRect;
			Int& infelicityRectStart = (m_IsVertical) ? infelicityRect.Top : infelicityRect.Left;
			Int& infelicityRectEnd = (m_IsVertical) ? infelicityRect.Bottom : infelicityRect.Right;

			const uInt cellCount = m_Cells.size();
			for (uInt i = 0; i < cellCount; ++i) {
				if (i == 0) {
					infelicityRectEnd = -gapInfelicity / 2;
				}
				else if ((i + 1) == cellCount) {
					infelicityRectStart = gapInfelicity / 2;
					infelicityRectEnd = -gapInfelicity / 2;
				}
				else {
					infelicityRectStart = gapInfelicity / 2;
					infelicityRectEnd = -gapInfelicity;
				}

				Cell& cell = m_Cells[i];
				cell.SetRect(cell.GetRect() + infelicityRect);
			}

			m_Gap = newGap;
		}
		void SetCellSize(const uInt& index, Float newSize, const UnitType& unit) {
			if (index >= m_Cells.size())
				Raise("Out of range");

			if (unit != UnitType::UNIT_PERCENTAGE) {
				const Int contentSize = (m_IsVertical) ? _GetContentRect().Bottom : _GetContentRect().Right;
				const Float cellDefaultSize = Float(contentSize / Int(m_LayoutSize));

				newSize = ConvertUnit(newSize, unit, UnitType::UNIT_PERCENTAGE, cellDefaultSize);
			}

			const uInt prevIndex = (index - 1);
			const uInt currentIndex = index;
			const uInt nextIndex = (index + 1);

			if (nextIndex >= m_Cells.size()) {
				if (newSize > m_CellsSchematic[prevIndex] + m_CellsSchematic[currentIndex])
					newSize = m_CellsSchematic[prevIndex] + m_CellsSchematic[currentIndex];
				else if (newSize < 0)
					newSize = 0;

				m_CellsSchematic[prevIndex] += m_CellsSchematic[currentIndex] - newSize;
				m_CellsSchematic[currentIndex] = newSize;
			}
			else {
				if (newSize > m_CellsSchematic[currentIndex] + m_CellsSchematic[nextIndex])
					newSize = m_CellsSchematic[currentIndex] + m_CellsSchematic[nextIndex];
				else if (newSize < 0)
					newSize = 0;

				m_CellsSchematic[nextIndex] += m_CellsSchematic[currentIndex] - newSize;
				m_CellsSchematic[currentIndex] = newSize;
			}

			UpdateContent();
		}

	private:
		std::vector<Cell> m_Cells;
		std::vector<Float> m_CellsSchematic;
		FloatRect m_PercentagePaddingRect;
		IntRect m_SavedClientRect;
		uInt m_LayoutSize;
		Int m_Gap;
		Bool m_IsVertical;

	private:
		virtual void _WMPaint(HDC& hdc, PAINTSTRUCT& paint) {
			if (IsVisibleDebugGrid) {
				constexpr uInt lineWeight = 1;

				_DrawPaddingRect(hdc, m_ClientRect, GetPaddingRect(), true);

				Int cellID = 0;
				for (const Cell& cell : m_Cells) {
					cell.Draw(hdc);

					_DrawPaddingRect(hdc, cell.GetRect(), cell.GetPaddingRect(), false);
					DrawFrameRect(hdc, cell.GetRect(), lineWeight, DebugGridColorConfig.FrameColor);

					if (cell.GetHandle() && cell.GetHandle()->GetHandle()) {
						const HDC childHDC = cell.GetHandle()->GetWindowDC();
						DrawFrameRect(childHDC, IntRect(Int2D(), cell.GetRect().RightBottom), lineWeight, DebugGridColorConfig.FrameColor);
					}

					const Bool isLastCell = (cell.GetRect().LeftTop == (m_Cells.end() - 1)->GetRect().LeftTop);
					if (m_Gap > 0 && (!isLastCell) && (cellID % 2) == 0) {
						Int drawGapPosition = (m_IsVertical) 
							? cell.GetRect().Top + cell.GetRect().Bottom
							: cell.GetRect().Left + cell.GetRect().Right;

						_DrawGapRect(hdc, m_ClientRect, GetPaddingRect(), drawGapPosition, m_Gap, (!m_IsVertical));
					}
					++cellID;
				}
				DrawFrameRect(hdc, m_ClientRect, lineWeight, DebugGridColorConfig.FrameColor);
			}
			else {
				for (const Cell& cell : m_Cells)
					cell.Draw(hdc);
			}
		}
		virtual void _WMCommand(const Long& param_1, const Long& param_2) override {
			SendMessage(m_hParent, WM_COMMAND, param_1, param_2);
		}

		IntRect _GetContentRect() const {
			IntRect contentRect = m_SavedClientRect;
			contentRect.LeftTop = GetPaddingRect().LeftTop;
			contentRect.RightBottom -= GetPaddingRect().LeftTop + GetPaddingRect().RightBottom;
			return contentRect;
		}
	};
}