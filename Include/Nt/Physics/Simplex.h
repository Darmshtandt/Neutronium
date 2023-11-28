#pragma once

namespace Nt {
	class Simplex {
	public:
		Simplex() :
			m_Size(0)
		{ }

		void Add(const Float3D& point) {
			m_Points[3] = m_Points[2];
			m_Points[2] = m_Points[1];
			m_Points[1] = m_Points[0];
			m_Points[0] = point;

			if (m_Size < 4)
				++m_Size;
		}

		const Float3D& operator [] (const uInt& index) const {
			if (index >= m_Size)
				Raise("Out of range");
			return m_Points[index];
		}
		Float3D& operator [] (const uInt& index) {
			if (index >= m_Size)
				Raise("Out of range");
			return m_Points[index];
		}
		Simplex& operator = (std::initializer_list<Float3D> list) {
			m_Size = min(4, list.size());
			for (uInt i = 0; i < m_Size; ++i)
				m_Points[i] = *(list.begin() + i);
			return *this;
		}

		uInt GetSize() const noexcept {
			return m_Size;
		}

	private:
		Float3D m_Points[4];
		uInt m_Size;
	};
}