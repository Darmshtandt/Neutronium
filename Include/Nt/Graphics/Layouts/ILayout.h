#pragma once

namespace Nt {
	struct ILayout {
		virtual void UpdateContent() noexcept = 0;
		virtual void ResetLayout() = 0;
	};
}