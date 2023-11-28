#pragma once

namespace Nt {
	class GridLayout : public Window, ILayout {
	public:
		class Column {
		public:
			Column() = default;

			void ResizeContainer(const uInt& newContainerSize) {
				if (m_Cells.size() == newContainerSize)
					return;
				
				m_Cells.resize(newContainerSize);
				SetRect(m_Rect);
			}

			constexpr std::vector<Cell>::iterator begin() noexcept {
				return m_Cells.begin();
			}
			constexpr std::vector<Cell>::iterator end() noexcept {
				return m_Cells.end();
			}
			constexpr std::vector<Cell>::const_iterator begin() const noexcept {
				return m_Cells.begin();
			}
			constexpr std::vector<Cell>::const_iterator end() const noexcept {
				return m_Cells.end();
			}

			Bool IsContained(const HandleWindow* pHandle) const noexcept {
				for (const Cell& cell : m_Cells)
					if (cell.GetHandle() == pHandle)
						return true;
				return false;
			}

			Cell& operator [] (const uInt& index) {
				if (index >= m_Cells.size())
					Raise("Out of range");
				return m_Cells[index];
			}

			uInt GetCellCount() const noexcept {
				return m_Cells.size();
			}
			IntRect GetRect() const noexcept {
				return m_Rect;
			}

			void SetRect(const IntRect& rect) {
				m_Rect = rect;

				if (m_Cells.size() == 0)
					return;

				IntRect newCellRect = m_Rect;
				newCellRect.Bottom /= m_Cells.size();
				for (uInt i = 0; i < m_Cells.size(); ++i) {
					m_Cells[i].SetRect(newCellRect);
					newCellRect.Top += newCellRect.Bottom;
				}
			}
			void SetPosition(const Int& x) noexcept {
				if (m_Rect.Left != x) {
					m_Rect.Left = x;
					for (Cell& cell : m_Cells)
						cell.SetPosition({ x, cell.GetRect().Top });
				}
			}
			void SetWidth(const Int& width) noexcept {
				if (m_Rect.Right != width) {
					m_Rect.Right = width;
					for (Cell& cell : m_Cells)
						cell.SetSize({ width, cell.GetRect().Bottom });
				}
			}

		private:
			std::vector<Cell> m_Cells;
			IntRect m_Rect;
		};

	public:
		GridLayout(const uInt2D& layoutSize) {
			SetLayoutSize(layoutSize);
		}
		GridLayout(const uInt2D& layoutSize, const IntRect& rect, const String& name) :
			Window(rect, name)
		{
			m_SavedClientRect = GetAdjustedWindowRect();
			SetLayoutSize(layoutSize);
		}
		GridLayout(const uInt2D& layoutSize, const Int2D& windowSize, const String& name) :
			Window(windowSize, name)
		{
			m_SavedClientRect = GetAdjustedWindowRect();
			SetLayoutSize(layoutSize);
		}
		
		void Create(const String& name) override {
			constexpr IntRect defaultWindowRect = 
				{ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
			Create(defaultWindowRect, name);
		}
		void Create(const Int2D& size, const String& name) override {
			if (IsCreated()) {
				Log::Warning("The handle has already been created");
				return;
			}

			Window::Create(size, name);

			m_SavedClientRect = GetAdjustedWindowRect();
			ResetLayout();
		}
		void Create(const IntRect& windowRect, const String& name) override {
			if (IsCreated()) {
				Log::Warning("The handle has already been created");
				return;
			}

			Window::Create(windowRect, name);

			m_SavedClientRect = GetAdjustedWindowRect();
			ResetLayout();
		}

		void UpdateContent() noexcept {
			const IntRect clientRect = GetAdjustedWindowRect();
			if (clientRect.RightBottom != m_SavedClientRect.RightBottom) {
				if (m_SavedClientRect.Right == 0)
					m_SavedClientRect.Right = 0;
				if (m_SavedClientRect.Bottom == 0)
					m_SavedClientRect.Bottom = 0;

				const LDouble2D differentSize =
					LDouble2D(clientRect.RightBottom) / LDouble2D(m_SavedClientRect.RightBottom);

				m_SavedClientRect = clientRect;
				for (Column& column : m_Columns) {
					for (Cell& cell : column)
						cell.SetRect(LDoubleRect(cell.GetRect()) * differentSize);
				}
			}
		}
		void ResetLayout() {
			const uInt2D windowClientSize = m_SavedClientRect.RightBottom;
			const uInt2D columnSize =
				{ windowClientSize.x / m_LayoutSize.x, windowClientSize.y };

			IntRect columnRect = { Int2D(), columnSize };
			for (uInt i = 0; i < m_LayoutSize.x; ++i) {
				m_Columns[i].SetRect(columnRect);
				columnRect.Left += columnRect.Right;
			}
		}

		void Insert(const uInt2D& index2D, HandleWindow* pHandle) {
			if (m_Columns.size() == 0 || index2D.x >= m_Columns.size() || index2D.y >= m_Columns[0].GetCellCount())
				Raise("Out of range");

			if (pHandle != nullptr)
				pHandle->SetParent(*this);

			for (const Column& column : m_Columns) {
				if (column.IsContained(pHandle)) {
					Log::Warning("Handle is already in GridLayout");
					return;
				}
			}

			m_Columns[index2D.x][index2D.y].SetHandle(pHandle);
		}

		void SetLayoutSize(const uInt2D& newLayoutSize) {
			if (m_LayoutSize == newLayoutSize)
				return;

			m_LayoutSize = newLayoutSize;
			if (m_LayoutSize.x == 0 || m_LayoutSize.y == 0) {
				m_Columns.clear();
				return;
			}

			const uInt2D windowClientSize = Window::GetAdjustedWindowRect().RightBottom;
			const uInt2D columnSize =
				{ windowClientSize.x / m_LayoutSize.x, windowClientSize.y };

			IntRect columnRect = { Int2D(), columnSize };

			m_Columns.resize(newLayoutSize.x);
			for (uInt i = 0; i < m_LayoutSize.x; ++i) {
				m_Columns[i].ResizeContainer(m_LayoutSize.y);
				m_Columns[i].SetRect(columnRect);
				columnRect.Left += columnRect.Right;
			}
		}
		void SetGap(Int2D newGap2D, const UnitType& unit) noexcept {
			if (unit == UnitType::UNIT_PERCENTAGE)
				newGap2D *= GetAdjustedWindowRect().RightBottom;
			if (m_Gap2D == newGap2D)
				return;

			for (uInt i = 0; i < m_Columns.size(); ++i) {
				const Int2D gapInfelicity = (newGap2D - m_Gap2D);

				IntRect infelicityRect;
				if (i == 0) {
					infelicityRect.Right = -gapInfelicity.x / 2;
				}
				else if ((i + 1) == m_Columns.size()) {
					infelicityRect.Left = gapInfelicity.x / 2;
					infelicityRect.Right = -gapInfelicity.x / 2;
				}
				else {
					infelicityRect.Left = gapInfelicity.x / 2;
					infelicityRect.Right = -gapInfelicity.x;
				}
				
				const uInt cellCount = m_Columns[i].GetCellCount();
				for (uInt j = 0; j < cellCount; ++j) {
					if (j == 0) {
						infelicityRect.Bottom = -gapInfelicity.y / 2;
					}
					else if ((j + 1) == cellCount) {
						infelicityRect.Top = gapInfelicity.y / 2;
						infelicityRect.Bottom = -gapInfelicity.y / 2;
					}
					else {
						infelicityRect.Top = gapInfelicity.y / 2;
						infelicityRect.Bottom = -gapInfelicity.y;
					}

					Cell& cell = m_Columns[i][j];
					cell.SetRect(cell.GetRect() + infelicityRect);
				}
			}

			m_Gap2D = newGap2D;
		}
		void SetColumnCount(const uInt& columnCount) {
			SetLayoutSize({ columnCount, m_LayoutSize.y });
		}
		void SetColumnWidth(const uInt& columnIndex, Int newColumnWidth, const UnitType& unit) {
			if (columnIndex >= m_Columns.size())
				Raise("Out of range");
			if (unit == UnitType::UNIT_PERCENTAGE)
				newColumnWidth *= Float(GetAdjustedWindowRect().Right) / m_Columns.size() / 100.f;

			const auto columnIterator = (m_Columns.begin() + columnIndex);
			const auto nextColumn = (columnIterator + 1);

			const Int oldColumnWidth = columnIterator->GetRect().Right;
			columnIterator->SetWidth(newColumnWidth);

			if (nextColumn == m_Columns.end()) {
				const auto prevColumn = (columnIterator - 1);

				const Int prevColumnWidth =
					prevColumn->GetRect().Right + (oldColumnWidth - newColumnWidth);
				const Int prevColumnPosition =
					prevColumn->GetRect().Left + prevColumnWidth;

				prevColumn->SetWidth(prevColumnWidth);
				columnIterator->SetPosition(prevColumnPosition);
			}
			else {
				const IntRect currentColumnRect = columnIterator->GetRect();

				const Int nextColumnPosition =
					currentColumnRect.Left + currentColumnRect.Right;
				const Int nextColumnWidth =
					nextColumn->GetRect().Right + (oldColumnWidth - newColumnWidth);

				nextColumn->SetPosition(nextColumnPosition);
				nextColumn->SetWidth(nextColumnWidth);
			}
		}
		void SetRowCount(const uInt& rowCount) {
			SetLayoutSize({ m_LayoutSize.x, rowCount });
		}
		void SetRowHeight(const uInt& rowIndex, Int newRowHeight, const UnitType& unit) {
			if (m_Columns.size() == 0 || rowIndex >= m_Columns[0].GetCellCount())
				Raise("Out of range");
			if (unit == UnitType::UNIT_PERCENTAGE)
				newRowHeight *= Float(GetAdjustedWindowRect().Bottom) / m_Columns.size() / 100.f;

			const uInt prevRowIndex = (rowIndex - 1);
			const uInt nextRowIndex = (rowIndex + 1);

			if (nextRowIndex == m_Columns[0].GetCellCount()) {
				const Int rowHeightDifferent = m_Columns[0][rowIndex].GetRect().Bottom - newRowHeight;
				const Int prevRowHeight = m_Columns[0][prevRowIndex].GetRect().Bottom;

				_ResizeRow(prevRowIndex, rowIndex, prevRowHeight + rowHeightDifferent);
			}
			else {
				_ResizeRow(rowIndex, nextRowIndex, newRowHeight);
			}
		}

		Column& operator [] (const uInt& index) {
			if (index >= m_Columns.size())
				Raise("Out of range");
			return m_Columns[index];
		}

	private:
		std::vector<Column> m_Columns;
		IntRect m_SavedClientRect;
		uInt2D m_LayoutSize;
		Int2D m_Gap2D;

	private:
		virtual void _WMPaint(HDC& hdc, PAINTSTRUCT& paint) {
			for (const Column& column : m_Columns) {
				for (const Cell& cell : column)
					cell.Draw(hdc);
			}
		}

	private:
		void _ResizeRow(const uInt& prevRowIndex, const uInt& nextRowIndex, const Int& newRowHeight) {
			for (Column& column : m_Columns) {
				if (prevRowIndex >= column.GetCellCount())
					Raise("Out of range");

				IntRect currentRowRect = column[prevRowIndex].GetRect();

				IntRect nextRowRect = column[nextRowIndex].GetRect();
				nextRowRect.Top = currentRowRect.Top + newRowHeight;
				nextRowRect.Bottom += currentRowRect.Bottom - newRowHeight;

				currentRowRect.Bottom = newRowHeight;

				column[prevRowIndex].SetSize(currentRowRect.RightBottom);
				column[nextRowIndex].SetRect(nextRowRect);
			}
		}
	};
}