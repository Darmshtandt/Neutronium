#pragma once

namespace Nt {
	class RigidBody : public IObject {
	public:
		RigidBody() noexcept :
			m_GravityDirection(Float3D::Down),
			m_G(9.81f),
			m_Mass(1.f),
			m_DeltaTime(0.f),
			m_Friction(0.01f),
			m_FrictionAir(0.01f),
			m_FrictionStatic(0.0f),
			m_IsEnable(true),
			m_IsObjectCollided(false),
			m_IsAwake(true),
			m_EnableCollision(true),
			m_EnableGravitation(false),
			m_IsActive(false)
		{
		}
		RigidBody(const RigidBody& rigidBody) :
			IObject(rigidBody),
			m_GravityDirection(rigidBody.m_GravityDirection),
			m_Force(rigidBody.m_Force),
			m_LinearAcceleration(rigidBody.m_LinearAcceleration),
			m_LinearVelocity(rigidBody.m_LinearVelocity),
			m_PrevPosition(rigidBody.m_PrevPosition),
			m_G(rigidBody.m_G),
			m_Mass(rigidBody.m_Mass),
			m_DeltaTime(rigidBody.m_DeltaTime),
			m_Friction(rigidBody.m_Friction),
			m_FrictionStatic(rigidBody.m_FrictionStatic),
			m_FrictionAir(rigidBody.m_FrictionAir),
			m_IsObjectCollided(rigidBody.m_IsObjectCollided),
			m_IsAwake(rigidBody.m_IsAwake),
			m_EnableCollision(rigidBody.m_EnableCollision),
			m_EnableGravitation(rigidBody.m_EnableGravitation),
			m_IsEnable(rigidBody.m_IsEnable),
			m_IsActive(rigidBody.m_IsActive)
		{
			for (ICollider* pCollider : rigidBody.m_Colliders)
				m_Colliders.push_back(pCollider->GetCopy());
		}
		~RigidBody() {
			RemoveAllColliders();
		}

		void Update(const Float& DeltaTime) {
			m_DeltaTime = DeltaTime;
			if (m_EnableGravitation)
				AddForce(m_GravityDirection / m_DeltaTime);

			if (m_IsActive && m_IsAwake) {
				m_IsAwake = false;
				AddForce(-m_LinearVelocity * (m_Friction + m_FrictionStatic));

				m_LinearAcceleration = m_Force / m_Mass;
				m_LinearVelocity += m_LinearAcceleration * m_DeltaTime;

				Translate(m_LinearVelocity * m_DeltaTime);
				m_Force = Float3D::Zero;

				//if (m_PrevPosition != GetPosition()) {
				//	for (ICollider* pCollider : m_Colliders)
				//		pCollider->Position = m_Position;
				//}
				//m_PrevPosition = GetPosition();
			}
		}

		Bool CheckCollision(const RigidBody* pBody) {
			m_IsObjectCollided = false;
			if (!pBody)
				Raise("pBody is nullptr");

			for (ICollider* pThisCollider : m_Colliders) {
				for (ICollider* pOtherCollider : pBody->m_Colliders) {
					if (!pThisCollider)
						Raise("This collider is nullptr");
					if (!pOtherCollider)
						Raise("Other collider is nullptr");

					if (pThisCollider->IsCollide(pOtherCollider))
						return true;
				}
			}
			return false;
		}

		void AddCollider(ICollider* pCollider) {
			switch (pCollider->Type) {
			case ICollider::TYPE_INVALID:
				Raise("Invalid collider type");
			case ICollider::TYPE_BOX:
			{
				BoxCollider* pBoxCollider = dynamic_cast<BoxCollider*>(pCollider);
				pBoxCollider->Position = GetPosition();
			}
				break;
			case ICollider::TYPE_SPHERE:
			{
				SphereCollider* pSphereCollider = dynamic_cast<SphereCollider*>(pCollider);
				pSphereCollider->Position = GetPosition();
			}
				break;
			case ICollider::TYPE_CAPSULE:
			{
				Raise("This type is not supported");
			}
				break;
			case ICollider::TYPE_TRIANGLE:
			{
				TriangleCollider* pTriangleCollider = dynamic_cast<TriangleCollider*>(pCollider);
				Raise("This type is not supported");
			}
				break;
			case ICollider::TYPE_MESH:
			{
				MeshCollider* pMeshCollider = dynamic_cast<MeshCollider*>(pCollider);
			}
				break;
			}
			m_Colliders.push_back(pCollider);
		}
		void RemoveCollider(const uInt& Index) {
			if (m_Colliders.size() <= Index)
				Raise("Out of range");

			auto iterator = m_Colliders.begin() + Index;
			delete(*iterator);
			m_Colliders.erase(iterator);
		}
		void RemoveAllColliders() {
			for (ICollider* pCollider : m_Colliders)
				delete(pCollider);
			m_Colliders.clear();
		}


		void AddForce(const Float3D& Force) noexcept {
			m_Force += Force * GetWeight();
			if (m_Force != Nt::Float3D::Zero)
				m_IsAwake = true;
		}

		void EnableGravitation() noexcept {
			m_EnableGravitation = true;
		}
		void DisableGravitation() noexcept {
			m_EnableGravitation = false;
		}
		void EnableCollider() noexcept {
			m_EnableCollision = true;
		}
		void DisableCollider() noexcept {
			m_EnableCollision = false;
		}
		void EnablePhysics() noexcept {
			m_IsEnable = true;
		}
		void DisablePhysics() noexcept {
			m_IsEnable = false;
			if (m_IsActive)
				m_IsActive = false;
		}
		void Activate() noexcept {
			if (m_IsEnable)
				m_IsActive = true;
		}
		void Deactivate() noexcept {
			m_IsActive = false;
		}


		uInt GetCollidersCount() const noexcept {
			return m_Colliders.size();
		}
		Float3D GetGravityDirection() const noexcept {
			return m_GravityDirection;
		}
		Float3D GetLinearVelocity() const noexcept {
			return m_LinearVelocity;
		}
		Float3D GetLinearAcceleration() const noexcept {
			return m_LinearAcceleration;
		}
		Float GetFriction() const noexcept {
			return m_Friction;
		}
		Float GetFrictionStatic() const noexcept {
			return m_FrictionStatic;
		}
		Float GetFrictionAir() const noexcept {
			return m_FrictionAir;
		}
		Float GetMass() const noexcept {
			return m_Mass;
		}
		Float GetWeight() const noexcept {
			return m_Mass * m_G;
		}
		Bool IsObjectCollided() const noexcept {
			return m_IsObjectCollided;
		}
		Bool IsEnabledCollision() const noexcept {
			return m_EnableCollision;
		}
		Bool IsEnabledGravitation() const noexcept {
			return m_EnableGravitation;
		}
		Bool IsActive() const noexcept {
			return m_IsActive;
		}
		Bool IsPhysicsEnabled() const noexcept {
			return m_IsEnable;
		}


		void SetGravityDirection(const Float3D& GravityDirection) noexcept {
			m_GravityDirection = GravityDirection.GetNormalize();
		}
		void SetLinearVelocity(const Float3D& LinearVelocity) noexcept {
			m_LinearVelocity = LinearVelocity;
		}
		void SetLinearAcceleration(const Float3D& LinearAcceleration) noexcept {
			m_LinearAcceleration = LinearAcceleration;
		}
		void SetForce(const Float3D& Force) noexcept {
			m_Force = Force;
		}
		void SetFriction(const Float& Friction) noexcept {
			m_Friction = Friction;
		}
		void SetFrictionStatic(const Float& FrictionStatic) noexcept {
			m_FrictionStatic = FrictionStatic;
		}
		void SetMass(const Float& Mass) noexcept {
			m_Mass = Mass;
		}

	protected:
		std::vector<ICollider*> m_Colliders;

		Float3D m_GravityDirection;
		Float3D m_Force;
		Float3D m_LinearAcceleration;
		Float3D m_LinearVelocity;
		Float3D m_PrevPosition;

		Float m_G;
		Float m_Mass;
		Float m_DeltaTime;
		Float m_Friction;
		Float m_FrictionStatic;
		Float m_FrictionAir;

		Bool m_IsObjectCollided;

	private:
		Bool m_IsEnable;
		Bool m_IsActive;
		Bool m_IsAwake;
		Bool m_EnableCollision;
		Bool m_EnableGravitation;
	};
}