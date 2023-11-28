#pragma once

namespace Nt {
	enum class UnitType {
		UNIT_PIXEL,
		UNIT_PERCENTAGE
	};

	class Cell {
	public:
		Cell(const IntRect& rect = { }, HandleWindow* pHandle = nullptr, const Bool& isClampToEdge = true) :
			m_CellRect(rect),
			m_pHandle(pHandle),
			m_IsClampToEdge(isClampToEdge),
			m_pText(nullptr)
		{
		}
		~Cell() {
			if (m_pHandle != nullptr)
				m_pHandle->SetParentHandle(nullptr);
		}

		void ToggleClampToEdge(const Bool& isClampToEdge) {
			m_IsClampToEdge = isClampToEdge;
			if (m_IsClampToEdge) {
				FloatRect newHandleRect = m_CellRect;
				newHandleRect.LeftTop += m_PercentagePadding.LeftTop;
				newHandleRect.RightBottom -= m_PercentagePadding.LeftTop + m_PercentagePadding.RightBottom;
				m_pHandle->SetWindowRect(newHandleRect);
			}
		}

		void Draw(const HDC& hdc) const {
			if (m_pText != nullptr)
				m_pText->Draw(hdc);
		}
		void Draw(const HandleWindow& handle) const {
			if (m_pText != nullptr)
				m_pText->Draw(handle);
		}

		HandleWindow* GetHandle() const noexcept {
			return m_pHandle;
		}
		IntRect GetRect() const noexcept {
			return m_CellRect;
		}
		FloatRect GetPercentagePaddingRect() const noexcept {
			return m_PercentagePadding;
		}

		void SetHandle(HandleWindow* pHandle) noexcept {
			m_pHandle = pHandle;
			_UpdateHandleRect();
		}
		void SetText(Text* pText) noexcept {
			m_pText = pText;
		}
		void SetPadding(const FloatRect& padding, const UnitType& unit) noexcept {
			switch (unit) {
			case UnitType::UNIT_PIXEL:
				m_PercentagePadding.LeftTop = m_CellRect.RightBottom / m_PercentagePadding.LeftTop;
				m_PercentagePadding.RightBottom = m_CellRect.RightBottom / m_PercentagePadding.RightBottom;
				break;
			case UnitType::UNIT_PERCENTAGE:
				m_PercentagePadding = padding;
				break;
			default:
				Log::Warning("Unknown unit type");
				break;
			}
			_UpdateHandleRect();
		}
		void SetPosition(const Int2D& position) noexcept {
			m_CellRect.LeftTop = position;
			_UpdateHandleRect();
		}
		void SetSize(const Int2D& size) noexcept {
			m_CellRect.RightBottom = size;
			_UpdateHandleRect();
		}
		void SetRect(const IntRect& newRect) noexcept {
			m_CellRect = newRect;
			_UpdateHandleRect();
		}

	private:
		Text* m_pText;
		HandleWindow* m_pHandle;
		IntRect m_CellRect;
		FloatRect m_PercentagePadding;
		Bool m_IsClampToEdge;

	private:
		void _UpdateHandleRect() noexcept {
			if (m_pHandle != nullptr && m_IsClampToEdge) {
				const IntRect padding = m_PercentagePadding * Float2D(m_CellRect.RightBottom);

				IntRect newRect = m_CellRect;
				newRect.LeftTop += padding.LeftTop;
				newRect.RightBottom -= padding.LeftTop + padding.RightBottom;
				m_pHandle->SetWindowRect(newRect);
			}
		}
	};
}