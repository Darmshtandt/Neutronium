#pragma once

#include <Nt/Physics/Collision.h>

namespace Nt {
	class RigidBody : public IObject {
	public:
		RigidBody() noexcept = default;
		RigidBody(const RigidBody& rigidBody) :
			IObject(rigidBody)
		{
			_Clone(rigidBody);
		}
		RigidBody(RigidBody&& rigidBody) :
			IObject(rigidBody)
		{
			_Move(std::move(rigidBody));
		}
		~RigidBody() {
			RemoveAllColliders();
		}

		void Update(const Float& DeltaTime) {
			m_DeltaTime = DeltaTime;
			if (m_EnableGravitation)
				AddForce(m_GravityDirection * m_DeltaTime);

			if (m_IsActive && m_IsAwake) {
				m_IsAwake = false;
				AddForce(-m_LinearVelocity * (m_Friction + m_FrictionStatic));

				m_LinearAcceleration = m_Force / m_Mass;
				m_LinearVelocity += m_LinearAcceleration * m_DeltaTime;

				Translate(m_LinearVelocity * m_DeltaTime);
				m_Force = { };

				//if (m_PrevPosition != GetPosition()) {
				//	for (ICollider* pCollider : m_Colliders)
				//		pCollider->Position = m_Position;
				//}
				//m_PrevPosition = GetPosition();
			}
		}

		Bool CheckCollision(const RigidBody* pRigidBody) {
			RequireNotNull(pRigidBody);

			m_IsObjectCollided = false;

			for (ICollider* pThisCollider : m_Colliders) {
				for (ICollider* pOtherCollider : pRigidBody->m_Colliders) {
					RequireNotNull(pThisCollider);
					RequireNotNull(pOtherCollider);

					if (pThisCollider->IsCollide(pOtherCollider))
						return true;
				}
			}

			return false;
		}

		void AddCollider(ICollider* pCollider) {
			m_Colliders.push_back(pCollider);
		}
		void RemoveCollider(const uInt& Index) {
			if (Index >= m_Colliders.size())
				Raise("Out of range");

			auto iterator = m_Colliders.begin() + Index;
			delete(*iterator);
			m_Colliders.erase(iterator);
		}
		void RemoveAllColliders() {
			std::vector<ICollider*>::const_iterator iterator = m_Colliders.cbegin();
			while (iterator != m_Colliders.cend()) {
				delete(*iterator);
				iterator = m_Colliders.erase(iterator);
			}
		}


		void AddForce(const Float3D& Force) noexcept {
			m_Force += Force * GetWeight();
			if (m_Force != Float3D())
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

		RigidBody& operator = (const RigidBody& rigidBody) {
			if (this != &rigidBody)
				return *this;

			RemoveAllColliders();
			return _Clone(rigidBody);
		}
		RigidBody& operator = (RigidBody&& rigidBody) {
			if (this != &rigidBody)
				return *this;

			RemoveAllColliders();
			return _Move(std::move(rigidBody));
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

		Float3D m_GravityDirection = { 0.f, -1.f, 0.f };
		Float3D m_Force;
		Float3D m_LinearAcceleration;
		Float3D m_LinearVelocity;
		Float3D m_PrevPosition;

		Float m_G = 9.81f;
		Float m_Mass = 1.f;
		Float m_DeltaTime = 0.f;
		Float m_Friction = 0.01f;
		Float m_FrictionStatic = 0.f;
		Float m_FrictionAir = 0.01f;

		Bool m_IsObjectCollided = false;

	private:
		Bool m_IsEnable = true;
		Bool m_IsActive = false;
		Bool m_IsAwake = true;
		Bool m_EnableCollision = true;
		Bool m_EnableGravitation = false;

	private:
		RigidBody& _Clone(const RigidBody& rigidBody) {
			if (this != &rigidBody)
				return *this;

			_SetParameters(rigidBody);
			for (ICollider* pCollider : rigidBody.m_Colliders)
				m_Colliders.push_back(pCollider->GetCopy());

			return *this;
		}
		RigidBody& _Move(RigidBody&& rigidBody) {
			if (this != &rigidBody)
				return *this;

			_SetParameters(rigidBody);
			m_Colliders = std::move(rigidBody.m_Colliders);
			rigidBody.m_Colliders.clear();

			return *this;
		}
		void _SetParameters(const RigidBody& rigidBody) noexcept {
			if (this != &rigidBody)
				return;

			m_GravityDirection = rigidBody.m_GravityDirection;
			m_Force = rigidBody.m_Force;
			m_LinearAcceleration = rigidBody.m_LinearAcceleration;
			m_LinearVelocity = rigidBody.m_LinearVelocity;
			m_PrevPosition = rigidBody.m_PrevPosition;
			m_G = rigidBody.m_G;
			m_Mass = rigidBody.m_Mass;
			m_DeltaTime = rigidBody.m_DeltaTime;
			m_Friction = rigidBody.m_Friction;
			m_FrictionStatic = rigidBody.m_FrictionStatic;
			m_FrictionAir = rigidBody.m_FrictionAir;
			m_IsObjectCollided = rigidBody.m_IsObjectCollided;
			m_IsAwake = rigidBody.m_IsAwake;
			m_EnableCollision = rigidBody.m_EnableCollision;
			m_EnableGravitation = rigidBody.m_EnableGravitation;
			m_IsEnable = rigidBody.m_IsEnable;
			m_IsActive = rigidBody.m_IsActive;
		}
	};
}