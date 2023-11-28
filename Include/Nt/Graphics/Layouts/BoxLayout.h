#pragma once

namespace Nt {
	class BoxLayout : public Window, ILayout {
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
			Create(defaultWindowRect, Name);
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
			const IntRect clientRect = GetClientRect();
			if (clientRect.RightBottom != m_SavedClientRect.RightBottom) {
				if (m_SavedClientRect.Right == 0)
					m_SavedClientRect.Right = 0;
				if (m_SavedClientRect.Bottom == 0)
					m_SavedClientRect.Bottom = 0;

				const LDouble2D differentSize =
					LDouble2D(clientRect.RightBottom) / LDouble2D(m_SavedClientRect.RightBottom);

				m_SavedClientRect = clientRect;
				for (Cell& cell : m_Cells)
					cell.SetRect(LDoubleRect(cell.GetRect()) * differentSize);
			}
		}
		void ResetLayout() {
			IntRect cellRect = m_SavedClientRect;
			if (m_IsVertical)
				cellRect.Bottom /= m_LayoutSize;
			else
				cellRect.Right /= m_LayoutSize;

			for (Cell& cell : m_Cells) {
				cell.SetRect(cellRect);

				if (m_IsVertical)
					cellRect.Top += cellRect.Bottom;
				else
					cellRect.Left += cellRect.Right;
			}
		}

		void Insert(const uInt& cellIndex, HandleWindow* pWindow) {
			if (cellIndex >= m_Cells.size())
				Raise("Out of range");

			if (pWindow != nullptr && pWindow->GetHandle() != nullptr) {
				const HWND handle = pWindow->GetHandle();
				for (const Cell& cell : m_Cells) {
					if (cell.GetHandle() != nullptr && cell.GetHandle()->GetHandle() == handle) {
						Log::Warning("This element has already been added to the BoxLayout");
						return;
					}
				}

				pWindow->SetParent(*this);
			}

			m_Cells[cellIndex].SetHandle(pWindow);
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

		void SetLayoutSize(const uInt& newLayoutSize) {
			if (m_LayoutSize != newLayoutSize) {
				m_LayoutSize = newLayoutSize;

				m_Cells.resize(m_LayoutSize);

				if (m_LayoutSize > 0) {
					IntRect cellRect = GetClientRect();
					if (m_IsVertical)
						cellRect.Bottom /= m_LayoutSize;
					else
						cellRect.Right /= m_LayoutSize;

					for (Cell& cell : m_Cells) {
						cell.SetRect(cellRect);

						if (m_IsVertical)
							cellRect.Top += cellRect.Bottom;
						else
							cellRect.Left += cellRect.Right;
					}
				}
				else {
					m_Cells.clear();
				}
			}
		}
		void SetCellSize(const uInt& index, Int newSize, const UnitType& unit) {
			if (index >= m_Cells.size())
				Raise("Out of range");

			if (unit == UnitType::UNIT_PERCENTAGE)
				newSize *= GetClientRect().Bottom;

			const uInt prevIndex = (index - 1);
			const uInt currentIndex = index;
			const uInt nextIndex = (index + 1);

			const IntRect oldCellRect = m_Cells[index].GetRect();
			IntRect newCellRect = oldCellRect;

			Int* pCurrentCellSize = &newCellRect.Right;
			if (m_IsVertical)
				pCurrentCellSize = &newCellRect.Bottom;

			const Int different = (m_IsVertical) 
				? (-oldCellRect.Bottom + newSize)
				: (-oldCellRect.Right + newSize);

			if (nextIndex >= m_Cells.size()) {
				IntRect prevCellRect = m_Cells[prevIndex].GetRect();

				Int* pPrevCellSize = &prevCellRect.Right;
				Int* pCurrentCellCoord = &newCellRect.Left;
				if (m_IsVertical) {
					pCurrentCellCoord = &newCellRect.Top;
					pPrevCellSize = &prevCellRect.Bottom;
				}

				(*pPrevCellSize) += different;
				(*pCurrentCellCoord) += different;
				(*pCurrentCellSize) -= different;

				m_Cells[prevIndex].SetRect(prevCellRect);
			}
			else {
				IntRect nextCellRect = m_Cells[nextIndex].GetRect();

				Int* pNextCellCoord = &nextCellRect.Left;
				Int* pNextCellSize = &nextCellRect.Right;
				if (m_IsVertical) {
					Int* pNextCellCoord = &nextCellRect.Top;
					Int* pNextCellSize = &nextCellRect.Bottom;
				}

				(*pCurrentCellSize) += different;
				(*pNextCellCoord) += different;
				(*pNextCellSize) -= different;

				m_Cells[nextIndex].SetRect(nextCellRect);
			}

			m_Cells[index].SetRect(newCellRect);
		}

		Cell& operator [] (const uInt& index) {
			if (index >= m_Cells.size())
				Raise("Out of range");
			return m_Cells[index];
		}

		Bool IsVertical() const noexcept {
			return m_IsVertical;
		}

	private:
		std::vector<Cell> m_Cells;
		IntRect m_SavedClientRect;
		uInt m_LayoutSize;
		Bool m_IsVertical;

	private:
		virtual void _WMPaint(HDC& hdc, PAINTSTRUCT& paint) {
			for (const Cell& cell : m_Cells)
				cell.Draw(hdc);
		}
	};
}