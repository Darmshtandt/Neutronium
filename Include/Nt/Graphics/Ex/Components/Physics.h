#pragma once

#include <Nt/Graphics/Ex/Components/Transform.h>
#include <Nt/Graphics/Ex/Physics/RigidBody.h>
#include <Nt/Graphics/Ex/Physics/BoundingBox.h>
#include <Nt/Graphics/Ex/Physics/PhysicBase.h>
#include <Nt/Core/Colors.h>
#include <functional>

namespace NtEx {
	using namespace Nt;

	class Collider final : public BaseComponent {
	public:
		struct Data final {
			const PointContainer& Points;
			const Matrix4x4& LocalWorld;
		};
		enum Axis {
			X, Y, Z
		};

	public:
		explicit Collider(Object* pOwner) :
			BaseComponent(pOwner, Class<Collider>::ID()),
			m_LocalTransform(pOwner)
		{
			m_pTransform = pOwner->GetComponent<TransformFloat3D>();
			if (m_pTransform == nullptr)
				m_pTransform = pOwner->AddComponent<TransformFloat3D>();
			m_LocalTransform.SetParent(m_pTransform);
		}

		NT_NODISCARD BoundingBox GetBoundingBox() const noexcept {
			if (m_IsDirty) {
				_UpdateData();
				m_IsDirty = false;
			}

			const Float3D position = m_pTransform->Position();

			return {
				.Min = m_BoundingBox.Min + position,
				.Max = m_BoundingBox.Max + position
			};
		}
		NT_NODISCARD const Mesh& GetForwardLineMesh() const noexcept {
			if (m_IsDirty) {
				_UpdateData();
				m_IsDirty = false;
			}

			return m_ForwardLine;
		}

		NT_NODISCARD Data GetData() const noexcept {
			return { m_HitBox.GetPoints(), m_LocalTransform.LocalToWorld() };
		}
		NT_NODISCARD const TransformFloat3D& LocalTransform() const noexcept {
			return m_LocalTransform;
		}
		NT_NODISCARD TransformFloat3D& LocalTransform() noexcept {
			return m_LocalTransform;
		}

		NT_NODISCARD const HitBox& GetHitBox() const noexcept {
			return m_HitBox;
		}
		NT_NODISCARD const Matrix4x4& LocalToWorld() const noexcept {
			return m_LocalTransform.LocalToWorld();
		}
		NT_NODISCARD const Matrix4x4& WorldToLocal() const noexcept {
			return m_LocalTransform.WorldToLocal();
		}
		NT_NODISCARD const PointContainer& GetPoints() const noexcept {
			return m_HitBox.GetPoints();
		}
		NT_NODISCARD uInt GetPointCount() const noexcept {
			return m_HitBox.GetPoints().size();
		}
		NT_NODISCARD Bool Enabled() const noexcept {
			return m_Enabled;
		}

		void SetShape(const Shape& shape) {
			m_HitBox.SetShape(shape);
			m_IsDirty = true;
		}

		void Enabled(const Bool enabled) noexcept {
			m_Enabled = enabled;
		}

	public:
		std::function<void(const PhysicObject&, const CollisionContact&)> OnEnter;
		std::function<void(const PhysicObject&, const CollisionContact&)> OnStay;
		std::function<void(const PhysicObject&)> OnExit;

	private:
		mutable BoundingBox m_BoundingBox;
		mutable Mesh m_ForwardLine;
		HitBox m_HitBox;
		TransformFloat3D* m_pTransform;
		TransformFloat3D m_LocalTransform;
		Bool m_Enabled = true;
		mutable Bool m_IsDirty = true;

	private:
		void _UpdateData() const {
			m_BoundingBox = CalculateBoundingBox(m_HitBox.GetPoints(), m_LocalTransform.LocalToWorld());

			const Float3D eye = { 0.f, m_BoundingBox.Max.y * 0.75f, 0.f };
			const Float3D forward = Float3D(0.f, 0.f, m_BoundingBox.Max.z + 1.f);

			Shape forwardLine;
			forwardLine.Indices = { 0, 1 };
			forwardLine.Vertices = {
				Vertex(eye, { }, { }, Colors::White),
				Vertex(eye + forward, { }, { }, Colors::White)
			};

			m_ForwardLine = forwardLine;
		}
	};

	class RigidBody final : public BaseComponent {
	public:
		explicit RigidBody(Object* pOwner) :
			BaseComponent(pOwner, Class<RigidBody>::ID())
		{
		}

		Base::RigidBody Body;
	};

	struct ITouchField {
		ITouchField(Object& object) {
			m_pTransform = object.GetComponent<TransformFloat3D>();
			Assert(m_pTransform != nullptr, "The Transform3D component is missing");
		}
		virtual ~ITouchField() noexcept = default;

		NT_NODISCARD virtual Bool BroadPhaseTest(const Collider& collider) const noexcept = 0;
		NT_NODISCARD virtual Bool NarrowPhaseTest(const Collider& collider) const noexcept = 0;

		std::function<void(Object&)> OnEnter;
		std::function<void(Object&)> OnIn;
		std::function<void(Object&)> OnExit;

	protected:
		TransformFloat3D* m_pTransform;
	};

	struct TouchSphere final : ITouchField {
		TouchSphere(Object& object) : ITouchField(object)
		{
		}
		~TouchSphere() noexcept override = default;

		NT_NODISCARD Bool BroadPhaseTest(const Collider& collider) const noexcept override {
			return true;
		}
		NT_NODISCARD Bool NarrowPhaseTest(const Collider& collider) const noexcept override {
			const PointContainer& colliderPoints = collider.GetPoints();
			for (const Float4D& point : colliderPoints) {
				const Float3D relativePosition = 
					point.xyz - m_pTransform->Position();
				if (relativePosition.LengthSquare() < Radius * Radius)
					return true;
			}

			return false;
		}

		Float Radius = 0.f;
	};

	class TouchTrigger final : public BaseComponent {
	public:
		explicit TouchTrigger(Object* pOwner) :
			BaseComponent(pOwner, Class<TouchTrigger>::ID())
		{
		}

	public:
		std::vector<ITouchField> m_Fields;
	};

	using ColliderPair = std::pair<const Collider::Data&, const Collider::Data&>;
}