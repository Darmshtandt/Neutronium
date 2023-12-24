#pragma once

namespace Nt {
	class GUI {
	public:
		GUI() = default;

		void Render(RenderWindow* pRenderWindow) const {
			if (pRenderWindow->IsEnabled3D()) {
				pRenderWindow->MatricesPush();
				pRenderWindow->DisableDepthBuffer();

				pRenderWindow->SetWorld(Matrix4x4::GetIdentity());
				pRenderWindow->SetView(Matrix4x4::GetIdentity());
				pRenderWindow->SetOrtho2DProjection();

				_RenderGUI(*pRenderWindow);

				pRenderWindow->EnableDepthBuffer();
				pRenderWindow->MatricesPop();
			}
			else {
				_RenderGUI(*pRenderWindow);
			}
		}

	private:
		virtual void _RenderGUI(const RenderWindow& Render2D) const = 0;
	};
}