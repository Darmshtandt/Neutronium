#pragma once

namespace Nt {
	class ICollider {
	public:
		enum eTypes {
			TYPE_INVALID,
			TYPE_BOX,
			TYPE_SPHERE,
			TYPE_CAPSULE,
			TYPE_TRIANGLE,
			TYPE_MESH
		};

		using PointContainer = std::vector<Nt::Float3D>;

	public:
		ICollider(const uInt& _Type) :
			Type(_Type) {
		}

		virtual Bool IsCollide(const ICollider* pCollider) const = 0;
		__inline void Render(Renderer* pRenderer) const {
			if (!pRenderer)
				Raise("Renderer is nullptr");

			if (m_pModel) {
				const Renderer::DrawingMode PrevDrawingMode = pRenderer->GetDrawingMode();
				pRenderer->SetDrawingMode(Renderer::DrawingMode::LINE_STRIP);
				pRenderer->SetLineWidth(5.f);
				pRenderer->DisableDepthBuffer();
				m_pModel->SetPosition(Position);
				m_pModel->Render(pRenderer);
				pRenderer->EnableDepthBuffer();
				pRenderer->SetDrawingMode(PrevDrawingMode);
			}
		}

		virtual ICollider* GetCopy() const = 0;

		__inline void SetColor(const Float4D& Color) noexcept {
			if (m_pModel)
				m_pModel->SetColor(Color);
		}

	public:
		Float3D Position;
		const uInt Type;

	protected:
		Model* m_pModel;
	};

	class BoxCollider : public ICollider {
	public:
		BoxCollider(Float3D Size) :
			ICollider(TYPE_BOX),
			m_Size(Size) {
			_RecreateModel();
		}
		
		NT_API Bool IsCollide(const ICollider* pCollider) const override final;

		ICollider* GetCopy() const override {
			return new BoxCollider(*this);
		}

		__inline void SetSize(const Float3D& Size) {
			m_Size = Size;
			_RecreateModel();
		}

	private:
		Float3D m_Size;

	private:
		__inline void _RecreateModel() {
			SAFE_DELETE(&m_pModel);
			if (m_Size.Length() > 0) {
				m_pModel = new Model(Geometry::Cube(m_Size, Colors::White));
				m_pModel->SetSize(m_Size);
				m_pModel->SetColor(Colors::Red);
			}
		}
	};

	struct SphereCollider : public ICollider {
		SphereCollider() :
			ICollider(TYPE_SPHERE),
			Radius(0.f) {
		}

		NT_API Bool IsCollide(const ICollider* pCollider) const override final;

		ICollider* GetCopy() const override {
			return new SphereCollider(*this);
		}

		Float Radius;
	};

	struct TriangleCollider : public ICollider {
		TriangleCollider() :
			ICollider(TYPE_TRIANGLE) {
		}

		NT_API Bool IsCollide(const ICollider* pCollider) const override final;

		void SetABC(const Float3D& A, const Float3D& B, const Float3D& C) noexcept {
			const Float3D AB = B - A;
			const Float3D BC = C - B;
			const Float3D CA = A - C;

			const Float ABLength = AB.LengthSquare();
			const Float BCLength = BC.LengthSquare();
			const Float CALength = CA.LengthSquare();
		}

		ICollider* GetCopy() const override {
			return new TriangleCollider(*this);
		}

		Matrix4x4 Mat;
	};

	class MeshCollider : public ICollider {
	public:
		MeshCollider() :
			ICollider(TYPE_MESH) {
		}

		NT_API Bool IsCollide(const ICollider* pCollider) const override final;

		[[nodiscard]]
		Float3D FindFurthestPoint(const Float3D& direction) const {
			Float3D maxPoint;
			Float maxDistance = -FLT_MAX;
			//Vec3D transformedDirection = (invModel() * direction).normalized();

			const Float3D transformedDirection = (m_LocalWorld * direction).GetNormalize();
			for (Float3D point : m_Points) {
				const Float pointLength = point.Dot(transformedDirection);
				if (pointLength > maxDistance) {
					maxDistance = pointLength;
					maxPoint = point;
				}
			}
			return maxPoint;
			//return model() * maxPoint + position();
		}
		[[nodiscard]]
		Float3D Support(const MeshCollider& collider, const Float3D& direction) const {
			return FindFurthestPoint(direction) - collider.FindFurthestPoint(-direction);
		}

		[[nodiscard]]
		Bool Line(Simplex& simplex, Float3D& direction) const {
			const Float3D a = simplex[0];
			const Float3D b = simplex[1];

			const Float3D ab = b - a;
			const Float3D ao = -a;

			if (ab.Dot(ao) > 0) {
				direction = ab.GetCross(ao).GetCross(ab);
			}
			else {
				simplex = { a };
				direction = ao;
			}
			return false;
		}
		[[nodiscard]]
		Bool Triangle(Simplex& simplex, Float3D& direction) const {
			const Float3D a = simplex[0];
			const Float3D b = simplex[1];
			const Float3D c = simplex[2];

			const Float3D ab = b - a;
			const Float3D ac = c - a;
			const Float3D ao = -a;

			const Float3D abc = ab.GetCross(ac);

			if (abc.GetCross(ac).Dot(ao) > 0) {
				if (ac.Dot(ao)) {
					simplex = { a, c };
					direction = ac.GetCross(ao).GetCross(ac);
				}
				else {
					simplex = { a, b };
					return Line(simplex, direction);
				}
			}
			else if (ab.GetCross(abc).Dot(ao) > 0) {
				simplex = { a, b };
				return Line(simplex, direction);
			}
			else {
				if (abc.Dot(ao) > 0) {
					direction = abc;
				}
				else {
					simplex = { a, c, b };
					direction = -abc;
				}
			}
			return false;
		}
		[[nodiscard]]
		Bool Polyhedron(Simplex& simplex, Float3D& direction) const {
			const Float3D a = simplex[0];
			const Float3D b = simplex[1];
			const Float3D c = simplex[2];
			const Float3D d = simplex[3];

			const Float3D ab = b - a;
			const Float3D ac = c - a;
			const Float3D ad = d - a;
			const Float3D ao = -a;

			const Float3D abc = ab.GetCross(ac);
			const Float3D acd = ac.GetCross(ad);
			const Float3D adb = ad.GetCross(ab);

			if (abc.Dot(ao) > 0) {
				simplex = { a, b, c };
				return Triangle(simplex, direction);
			}
			else if (acd.Dot(ao) > 0) {
				simplex = { a, c, d };
				return Triangle(simplex, direction);
			}
			else if (adb.Dot(ao) > 0) {
				simplex = { a, d, b };
				return Triangle(simplex, direction);
			}

			return true;
		}

		[[nodiscard]]
		Bool NextSimplex(Simplex& simplex, Float3D& direction) const {
			switch (simplex.GetSize()) {
			case 2:
				return Line(simplex, direction);
			case 3:
				return Triangle(simplex, direction);
			case 4:
				return Polyhedron(simplex, direction);
			}
			return true;
		}

		[[nodiscard]]
		Bool GJK(const MeshCollider& collider) const {
			Simplex simplex;
			simplex.Add(Support(collider, { 1.f, 0.f, 0.f }));

			Float3D direction = -simplex[0];
			//while (iters++ < size() + obj->size()) {
			for (uInt i = 0; i < m_Points.size() + collider.m_Points.size(); ++i) {
				Float3D point = Support(collider, direction);
				if (point.Dot(direction) <= 0)
					return false;

				simplex.Add(point);

				if (NextSimplex(simplex, direction))
					return true;

				//NextSimplex nextSimplex = _nextSimplex(points);
			}
			return true;
		}

		Bool RayIntersectTriangleTest(const Ray& ray, const Float3D face[3]) const {
			const Float3D ab = face[1] - face[0];
			const Float3D ac = face[2] - face[0];

			Float3D P = ray.Direction().GetCross(ac);
			Float determinant = ab.Dot(P);
			if (std::abs(determinant) < 0.000001)
				return false;

			Float inverseDeterminant = 1.0 / determinant;
			Float3D rayStartToA = ray.Start - face[0];

			Float intersectionParam1 = rayStartToA.Dot(P) * inverseDeterminant;
			if (intersectionParam1 < 0 || intersectionParam1 > 1)
				return false;

			const Float3D qvec = rayStartToA.GetCross(ab);

			Float intersectionParam2 = ray.Direction().Dot(qvec) * inverseDeterminant;
			if (intersectionParam2 < 0 || intersectionParam1 + intersectionParam2 > 1)
				return false;
			return true;
		}

		Bool RayCastTest(const Ray& ray) const {
			for (uInt i = 2; i < m_Points.size(); i += 3) {
				const Float3D face[3] = {
					m_Points[i - 2],
					m_Points[i - 1],
					m_Points[i - 0]
				};
				if (RayIntersectTriangleTest(ray, face))
					return true;
			}
			return false;
		}

		void SetLocalWorld(const Nt::Matrix4x4& localWorld) noexcept {
			m_LocalWorld = localWorld;
		}
		void SetShape(const Shape& shape) {
			m_Points.clear();
			if (shape.Indices.size() == 0) {
				for (Index_t index : shape.Indices)
					m_Points.push_back(shape.Vertices[index].Position);
			}
			else {
				for (Vertex vertex : shape.Vertices)
					m_Points.push_back(vertex.Position);
			}
		}
		void SetPointContainer(const PointContainer& points) {
			m_Points = points;
		}

		const PointContainer& GetPointContainer() const {
			return m_Points;
		}
		ICollider* GetCopy() const override {
			return new MeshCollider(*this);
		}

	private:
		PointContainer m_Points;
		Nt::Matrix4x4 m_LocalWorld;
	};


	__inline Bool AABBCollisionCheck(const Float3D& MinA, const Float3D& MaxA, const Float3D& MinB, const Float3D& MaxB) noexcept {
		return (MaxA - MinB >= 0 && MaxB - MinA >= 0);
	}
}