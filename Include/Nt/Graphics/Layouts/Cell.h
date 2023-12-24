#pragma once

namespace Nt {
	enum class UnitType {
		UNIT_PIXEL,
		UNIT_PERCENTAGE,
		UNIT_COUNT
	};

	template <typename _Ty, typename _U>
	_Ty ConvertUnit(const _Ty& value, const UnitType& unitFrom, const UnitType& unitTo, const _U& maxValue) noexcept {
		static_assert(Int(UnitType::UNIT_COUNT) == 2, "No new unit types have been added");

		if (unitFrom == UnitType::UNIT_PERCENTAGE && maxValue == 0) {
			Log::Warning("Invalid maximum value passed");
			return value;
		}
		else if (unitFrom == unitTo) {
			Log::Warning("2 identical units of measurement were transmitted");
			return value;
		}

		switch (unitTo) {
		case UnitType::UNIT_PERCENTAGE:
			return (value / maxValue);
		case UnitType::UNIT_PIXEL:
			return (value * maxValue);
		}

		Log::Warning("Unknown unit type");
		return value;
	}

	template <template <typename ...> class _C, typename _Ty, template <typename ...> class _Cl, typename _U>
	_C<_Ty> ConvertUnit(const _C<_Ty>& value, const UnitType& unitFrom, const UnitType& unitTo, const _Cl<_U>& maxValue) {
		const uInt maxValueDataCount = sizeof(maxValue) / sizeof(_U);

		_C<_Ty> newValue;
		if constexpr (Is_Same_Template_v<_C, Rect> || Is_Same_Template_v<_C, Vector4D>) {
			for (uInt i = 0; i < 4; ++i)
				newValue[i] = ConvertUnit<_Ty>(value[i], unitFrom, unitTo, maxValue[i % maxValueDataCount]);
		}
		else if constexpr (Is_Same_Template_v<_C, Vector3D>) {
			for (uInt i = 0; i < 3; ++i)
				newValue[i] = ConvertUnit<_Ty>(value[i], unitFrom, unitTo, maxValue[i % maxValueDataCount]);
		}
		else if constexpr (Is_Same_Template_v<_C, Vector2D>) {
			for (uInt i = 0; i < 2; ++i)
				newValue[i] = ConvertUnit<_Ty>(value[i], unitFrom, unitTo, maxValue[i % maxValueDataCount]);
		}
		return newValue;
	}

	class Cell {
	public:
		Cell(const IntRect& rect = { }, HandleWindow* pHandle = nullptr, const Bool& isClampToEdge = true) :
			m_Rect(rect),
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
				FloatRect newHandleRect = m_Rect;
				newHandleRect.LeftTop += m_PercentagePaddingRect.LeftTop;
				newHandleRect.RightBottom -= m_PercentagePaddingRect.LeftTop + m_PercentagePaddingRect.RightBottom;
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
		Text* GetTextPtr() const noexcept {
			return m_pText;
		}
		IntRect GetRect() const noexcept {
			return m_Rect;
		}
		FloatRect GetPercentagePaddingRect() const noexcept {
			return m_PercentagePaddingRect;
		}
		FloatRect GetPaddingRect() const noexcept {
			return m_PercentagePaddingRect * Float2D(m_Rect.RightBottom);
		}

		void SetHandle(HandleWindow* pHandle) noexcept {
			m_pHandle = pHandle;
			_UpdateContentRect();
		}
		void SetText(Text* pText) noexcept {
			m_pText = pText;
			_UpdateContentRect();
		}
		void SetPadding(const FloatRect& paddingRect, const UnitType& unit) noexcept {
			FloatRect newPercentagePaddingRect = paddingRect;
			if (unit != UnitType::UNIT_PERCENTAGE)
				newPercentagePaddingRect = ConvertUnit(paddingRect, unit, UnitType::UNIT_PERCENTAGE, Float2D(m_Rect.RightBottom));

			if (m_PercentagePaddingRect != newPercentagePaddingRect) {
				m_PercentagePaddingRect = newPercentagePaddingRect;
				_UpdateContentRect();
			}
		}
		void SetPosition(const Int2D& position) noexcept {
			m_Rect.LeftTop = position;
			_UpdateContentRect();
		}
		void SetSize(const Int2D& size) noexcept {
			m_Rect.RightBottom = size;
			_UpdateContentRect();
		}
		void SetRect(const IntRect& newRect) noexcept {
			m_Rect = newRect;
			_UpdateContentRect();
		}

	private:
		Text* m_pText;
		HandleWindow* m_pHandle;
		IntRect m_Rect;
		FloatRect m_PercentagePaddingRect;
		Bool m_IsClampToEdge;

	private:
		void _UpdateContentRect() noexcept {
			const IntRect padding = m_PercentagePaddingRect * Float2D(m_Rect.RightBottom);

			IntRect newRect = m_Rect;
			newRect.LeftTop += padding.LeftTop;

			if (m_IsClampToEdge)
				newRect.RightBottom -= padding.LeftTop + padding.RightBottom;
			else
				newRect.RightBottom = m_pHandle->GetWindowRect().RightBottom;

			if (m_pHandle != nullptr)
				m_pHandle->SetWindowRect(newRect);
			if (m_pText != nullptr)
				m_pText->SetRect(newRect);
		}
	};
}