#pragma once

#include <Nt/Graphics/Ex/Components/Transform.h>

namespace NtEx {
	using namespace Nt;

	struct Ray;
	
	class RigidBody;
	class Collider;

	struct PhysicObject {
		TransformFloat3D* pTransform = nullptr;
		RigidBody* pRigidBody = nullptr;
		Collider* pCollider = nullptr;
		Object* pObject = nullptr;
	};

	struct CollisionContact final {
		Float3D ContactPoint;
		Float3D Normal;
		Float Depth;
	};
	struct ObjectContactPair final {
		void OnEnter() const;
		void OnStay() const;
		void OnExit() const;

		const PhysicObject* pA;
		const PhysicObject* pB;
		CollisionContact Contact;
	};

	struct PhysicObjectPair final {
		PhysicObjectPair() noexcept = default;
		PhysicObjectPair(const ObjectContactPair& contact) noexcept :
			pA(contact.pA),
			pB(contact.pB) {
		}
		PhysicObjectPair(const PhysicObject* a, const PhysicObject* b) noexcept :
			pA(a),
			pB(b) {
		}

		NT_NODISCARD Bool operator == (const PhysicObjectPair& other) const noexcept {
			return (pA == other.pA && pB == other.pB)
				|| (pA == other.pB && pB == other.pA);
		}
		NT_NODISCARD Bool operator == (const ObjectContactPair& contact) const noexcept {
			return (pA == contact.pA && pB == contact.pB)
				|| (pA == contact.pB && pB == contact.pA);
		}

		const PhysicObject* pA;
		const PhysicObject* pB;
	};

	struct PhysicObjectHash final {
		NT_NODISCARD std::size_t operator () (const PhysicObjectPair& pair) const noexcept {
			return
				reinterpret_cast<std::size_t>(pair.pA) ^
				reinterpret_cast<std::size_t>(pair.pB);
		}
	};


	struct IPhysicsIntegrator {
		virtual ~IPhysicsIntegrator() noexcept = default;
		virtual void Integrate(const PhysicObject& object, const Float& deltaTime) const = 0;
		virtual void SetGravityVelocity(const Float3D& gravity) noexcept = 0;
	};

	struct IBroadPhaseDetector {
		virtual ~IBroadPhaseDetector() noexcept = default;
		NT_NODISCARD
		virtual std::vector<PhysicObjectPair> ComputePairs(const std::vector<PhysicObject>& bodies) const = 0;
	};

	struct INarrowPhaseDetector {
		virtual ~INarrowPhaseDetector() noexcept = default;
		NT_NODISCARD
		virtual std::vector<ObjectContactPair> GenerateContacts(const std::vector<PhysicObjectPair>& pairs) const = 0;
	};

	struct ICollisionResolver {
		virtual ~ICollisionResolver() noexcept = default;
		virtual void Resolve(const ObjectContactPair& contact) const = 0;
	};

	struct IPhysicsOverlapper {
		virtual ~IPhysicsOverlapper() noexcept = default;

		NT_NODISCARD
		virtual std::vector<PhysicObject> OverlapSphere(const Float3D& position, const Float& radius, const Int& layerMask) = 0;
		NT_NODISCARD
		virtual std::vector<PhysicObject> OverlapAABB(const Float3D& min, const Float3D& max, const Int& layerMask) = 0;
		NT_NODISCARD
		virtual std::vector<PhysicObject> OverlapRayCast(const Ray& ray, const Int& layerMask) = 0;
	};
}