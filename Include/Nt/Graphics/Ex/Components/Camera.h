#pragma once

#include <Nt/Graphics/Ex/Components/Transform.h>

namespace NtEx {
	using namespace Nt;

	struct ICamera {
		virtual ~ICamera() noexcept = default;

		NT_NODISCARD virtual Matrix4x4 View() const noexcept = 0;
		NT_NODISCARD virtual Bool IsDirty() const noexcept = 0;
		NT_NODISCARD virtual uInt GetDimension() const noexcept = 0;
	};

	template <typename _Ty, uInt Dimension>
	class Camera final : public BaseComponent, public ICamera {
	public:
		using Transform = Transform<_Ty, Dimension>;
		using Vector = typename Transform::Vector;

	public:
		explicit Camera(Object* pOwner) :
			BaseComponent(pOwner, Class<Camera>::ID()),
			m_LocalTransform(pOwner)
		{
			m_pParentTransform = pOwner->GetComponent<Transform>();
			if (m_pParentTransform == nullptr)
				m_pParentTransform = pOwner->AddComponent<Transform>();
			m_LocalTransform.SetParent(m_pParentTransform);
		}
		~Camera() noexcept override = default;

		NT_NODISCARD Matrix4x4 View() const noexcept override {
			return m_LocalTransform.WorldToLocal();
		}

		NT_NODISCARD Transform& LocalTransform() noexcept {
			return m_LocalTransform;
		}
		NT_NODISCARD uInt GetDimension() const noexcept override {
			return Dimension;
		}
		NT_NODISCARD Bool IsDirty() const noexcept override {
			return m_pParentTransform->IsDirty();
		}

	private:
		Transform* m_pParentTransform;
		Transform m_LocalTransform;
	};

	using Camera2D = Camera<Float, 2>;
	using Camera3D = Camera<Float, 3>;
}