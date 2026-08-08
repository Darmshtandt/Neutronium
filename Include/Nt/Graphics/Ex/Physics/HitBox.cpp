#include <Nt/Graphics/Ex/Physics/HitBox.h>
#include <Nt/Core/Colors.h>

namespace NtEx {
	using namespace Nt;

	void HitBox::Show() noexcept {
		m_IsVisible = true;
	}

	void HitBox::Hide() noexcept {
		m_IsVisible = false;
	}

	void HitBox::ToggleVisible(const Bool& enabled) noexcept {
		m_IsVisible = enabled;
	}

	void HitBox::ToggleVisible() noexcept {
		m_IsVisible = !m_IsVisible;
	}

	void HitBox::SetShape(const Shape& shape) {
		m_Points.clear();
		if (shape.Indices.empty()) {
			m_Points.reserve(shape.Vertices.size());
			for (const Vertex& vertex : shape.Vertices)
				m_Points.emplace_back(vertex.Position);
		}
		else {
			m_Points.reserve(shape.Indices.size());
			for (const Index_t index : shape.Indices)
				m_Points.emplace_back(shape.Vertices[index].Position);
		}

		m_Mesh.SetShape(shape);
	}

	void HitBox::SetPointContainer(const PointContainer& points) {
		m_Points = points;

		Shape shape;
		for (const Float3D& point : m_Points) {
			Vertex vertex = { };
			vertex.Position.xyz = point;
			vertex.Color = Colors::White;

			shape.Vertices.push_back(vertex);
		}

		m_Mesh.SetShape(shape);
	}

	const PointContainer& HitBox::GetPoints() const noexcept {
		return m_Points;
	}
	const Mesh* HitBox::GetMeshPtr() const noexcept {
		return &m_Mesh;
	}
	Bool HitBox::IsVisible() const noexcept {
		return m_IsVisible;
	}
}