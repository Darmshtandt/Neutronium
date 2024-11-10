#pragma once

#ifdef NT_EXPEREMENTAL
namespace Nt {
	struct Tetrahedron {
		Vertex Vertices[4];
	};

	void AddSuperTetrahedron(Shape& shape, const Vertices_t& sortedVertices) {
		if (shape.Vertices.size() < 3)
			return;

		Float3D minPoint;
		minPoint.x = sortedVertices.front().Position.x;
		minPoint.y = FLT_MAX;
		minPoint.z = FLT_MAX;

		Float3D maxPoint;
		maxPoint.x = sortedVertices.back().Position.x;
		maxPoint.y = -FLT_MAX;
		maxPoint.z = -FLT_MAX;

		for (const Vertex& vertex : sortedVertices) {
			if (minPoint.y > vertex.Position.y)
				minPoint.y = vertex.Position.y;
			if (minPoint.z > vertex.Position.z)
				minPoint.z = vertex.Position.z;

			if (maxPoint.y < vertex.Position.y)
				maxPoint.y = vertex.Position.y;
			if (maxPoint.z < vertex.Position.z)
				maxPoint.z = vertex.Position.z;
		}

		minPoint *= 10.f;
		maxPoint *= 10.f;

		const Float3D middlePoint = (minPoint + maxPoint) / 2.f;

		Vertex newVertex;
		newVertex.Position = { minPoint.x, minPoint.y, minPoint.z };
		shape.Indices.push_back(shape.Vertices.size());
		shape.Vertices.push_back(newVertex);

		newVertex.Position = { maxPoint.x, minPoint.y, minPoint.z };
		shape.Indices.push_back(shape.Vertices.size());
		shape.Vertices.push_back(newVertex);

		newVertex.Position = { middlePoint.x, minPoint.y, maxPoint.z };
		shape.Indices.push_back(shape.Vertices.size());
		shape.Vertices.push_back(newVertex);

		newVertex.Position = { middlePoint.x, maxPoint.y, middlePoint.z };
		shape.Indices.push_back(shape.Vertices.size());
		shape.Vertices.push_back(newVertex);
	}

	Bool TetrahedronCircumcircles(const Tetrahedron& Tetrahedron, const Float3D& point) {
		const Float3D ap = Tetrahedron.Vertices[0].Position - point;
		const Float3D bp = Tetrahedron.Vertices[1].Position - point;
		const Float3D cp = Tetrahedron.Vertices[2].Position - point;
		const Float3D dp = Tetrahedron.Vertices[3].Position - point;

		const Matrix4x4 matrix = {
			ap.x, ap.y, ap.z, ap.LengthSquare(),
			bp.x, bp.y, bp.z, bp.LengthSquare(),
			cp.x, cp.y, cp.z, cp.LengthSquare(),
			dp.x, dp.y, dp.z, dp.LengthSquare(),
		};

		return (matrix.Det() > 0);
	}

	Shape DelaunayTriangulate3D(const Vertices_t& vertices) {
		Shape shape;
		shape.Vertices = vertices;

		if (vertices.size() < 3)
			return Shape();

		Vertices_t sortedVertices = vertices;
		std::sort(sortedVertices.begin(), sortedVertices.end(),
			[](const Vertex& vertex_1, const Vertex& vertex_2) {
				return (vertex_1.Position.x < vertex_2.Position.x);
			});

		AddSuperTetrahedron(shape, sortedVertices);

		for (const Vertex& vertex : sortedVertices) {
			shape.Vertices.push_back(vertex);

			std::vector<Tetrahedron> notBadTetrahedrons;
			std::vector<Tetrahedron> badTetrahedrons;
			for (uInt i = 3; i < shape.Vertices.size(); ++i) {
				Tetrahedron tetrahedron;
				tetrahedron.Vertices[0] = shape.Vertices[i - 3];
				tetrahedron.Vertices[1] = shape.Vertices[i - 2];
				tetrahedron.Vertices[2] = shape.Vertices[i - 1];
				tetrahedron.Vertices[3] = shape.Vertices[i - 0];

				if (TetrahedronCircumcircles(tetrahedron, vertex.Position))
					badTetrahedrons.push_back(tetrahedron);
				else
					notBadTetrahedrons.push_back(tetrahedron);
			}
		}
		return shape;
	}
}
#endif