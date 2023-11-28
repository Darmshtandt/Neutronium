#pragma once

namespace Nt {
	class PhysicalWorld {
	private:
		const Float M_TO_METERS = 100.f;

	public:
		PhysicalWorld() :
			m_G(9.81f),
			m_FrictionAir(0.01f) {
		}

		void Update(const Float& Time) noexcept {

		}

		void CheckCollisions(const PhysicalWorld* pPhysicalWorld) {

		}

		Float GetG() const noexcept {
			return m_G;
		}
		Float GetFrictionAir() const noexcept {
			return m_FrictionAir;
		}


		void SetG(const Float& G) noexcept {
			m_G = G;
		}
		void SetFrictionAir(const Float& FrictionAir) noexcept {
			m_FrictionAir = FrictionAir;
		}

	private:
		std::vector<RigidBody*> m_pObjects;
		Float m_G = 9.81f;
		Float m_FrictionAir = 0.01f;
	};
}