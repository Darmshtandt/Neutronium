#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	using Polytope = std::vector<Float3D>;

	class Simplex final {
	public:
		Simplex() noexcept = default;
		Simplex(const Simplex&) noexcept = default;
		Simplex(Simplex&&) noexcept = default;
		~Simplex() noexcept = default;

		Simplex& operator = (const Simplex&) noexcept = default;
		Simplex& operator = (Simplex&&) noexcept = default;

		void Add(const Float3D& point) {
			m_Points[3] = m_Points[2];
			m_Points[2] = m_Points[1];
			m_Points[1] = m_Points[0];
			m_Points[0] = point;

			if (m_Size < 4)
				++m_Size;
		}

		NT_NODISCARD Polytope ToPolytope() const {
			if (m_Size < 1)
				return { };

			Polytope polytope;
			polytope.reserve(m_Size);
			for (uInt i = 0; i < m_Size; ++i)
				polytope.emplace_back(m_Points[i]);

			return polytope;
		}

		NT_NODISCARD const Float3D& operator [] (const uInt& index) const {
			if (index >= m_Size)
				Raise("Out of range");
			return m_Points[index];
		}
		NT_NODISCARD Float3D& operator [] (const uInt& index) {
			if (index >= m_Size)
				Raise("Out of range");
			return m_Points[index];
		}
		Simplex& operator = (std::initializer_list<Float3D> list) {
			m_Size = std::min(4u, list.size());
			for (uInt i = 0; i < m_Size; ++i)
				m_Points[i] = *(list.begin() + i);
			return *this;
		}

		NT_NODISCARD uInt GetSize() const noexcept {
			return m_Size;
		}

	private:
		Float3D m_Points[4];
		uInt m_Size = 0;
	};
}