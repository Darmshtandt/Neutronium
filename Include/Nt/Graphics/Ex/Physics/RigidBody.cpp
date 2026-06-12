#include <Nt/Graphics/Ex/Physics/RigidBody.h>

namespace NtEx::Base {
	using namespace Nt;

	void RigidBody::ApplyForces(const Float& deltaTime) {
		if (m_Mass <= 0.f)
			return;

		const Float3D linearAcceleration = m_Force / m_Mass;
		m_LinearVelocity += linearAcceleration * deltaTime * 1.f;
		m_LinearVelocity *= std::max(0.f, 1.f - m_LinearDamping * deltaTime);

		m_Force = { };
	}

	void RigidBody::AddForce(const Float3D& force) noexcept {
		m_Force += force;
	}

	Float3D RigidBody::GetLinearVelocity() const noexcept {
		return m_LinearVelocity;
	}

	Float3D RigidBody::GetForce() const noexcept {
		return m_Force;
	}

	Float RigidBody::GetMass() const noexcept {
		return m_Mass;
	}

	Float RigidBody::GetRestitution() const noexcept {
		return m_Restitution;
	}

	Float RigidBody::GetLinearDamping() const noexcept {
		return m_LinearDamping;
	}

	Bool RigidBody::HasForce() const noexcept {
		return m_Force.LengthSquare() > 0.f;
	}
	Bool RigidBody::IsStatic() const noexcept {
		return m_Mass <= 0.f;
	}

	void RigidBody::SetLinearVelocity(const Float3D& linearVelocity) noexcept {
		m_LinearVelocity = linearVelocity;
	}
	void RigidBody::SetForce(const Float3D& force) noexcept {
		m_Force = force;
	}
	void RigidBody::SetMass(const Float& mass) noexcept {
		m_Mass = mass;
	}

	void RigidBody::SetRestitution(const Float& restitution) noexcept {
		m_Restitution = Clamp(restitution, 0.f, 1.f);
	}

	void RigidBody::SetLinearDamping(const Float& linearDamping) noexcept {
		m_LinearDamping = linearDamping;
	}
}