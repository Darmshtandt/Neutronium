#pragma once

#ifdef NT_EXPEREMENTAL
namespace Nt {
	template <typename _Ty>
	class TableSet {
	public:
		using Column = _Ty*;
		using Row = _Ty*;

	public:
		TableSet() = default;
		TableSet(const uint2D size) : m_Size(size) {
			if (size.x != 0 && size.y != 0) {
				m_pDataArray = new _Ty[size.x * size.y];
				m_pColumns = new Column[size.x];
				m_pRows = new Row[size.y];


			}
		}

	private:
		_Ty* m_pDataArray = nullptr;
		Column* m_pColumns = nullptr;
		Row* m_pRows = nullptr;
		uInt2D m_Size;
	};
}
#endif