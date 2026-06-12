#pragma once

#include <Nt/Core/Math/Vector.h>

namespace Nt {
	class NT_API Listener {
	public:
		NT_NODISCARD const Float3D& GetPosition() const noexcept;
		NT_NODISCARD const Float3D& GetRotation() const noexcept;

		void SetPosition(const Float3D& position);
		void SetRotation(const Float3D& rotation);

	private:
		Float3D m_Position;
		Float3D m_Rotation;
	};
}