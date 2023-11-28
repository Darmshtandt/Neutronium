#include <NtStdH.h>
#include <Nt/Graphics.h>

#pragma comment(lib, "NeutroniumCore32d")
#pragma comment(lib, "NeutroniumGraphics32d")

int main() {
	try {
		Nt::RenderWindow Window({ 800, 600 }, L"Cube", true);
		 
		Nt::Camera Camera(Nt::Float3D::Back, Nt::Float3D::Zero);
		Camera.SetPosition({ 0.f, 0.f, -5.f });
		Window.SetCurrentCamera(&Camera);

		constexpr Nt::Float3D CubeSize = { 1.f, 1.f, 1.f };
		Nt::Model Cube(Nt::Geometry::Cube(CubeSize, Nt::Colors::White));
		Cube.SetTexture(Nt::Texture("Logo.tga"));

		const Nt::Float3D RotateSpeed(100.f, 100.f, 100.f);

		Window.SetClearColor(Nt::Colors::LightBlue);
		Window.Show();

		Nt::Event Event;
		while (Window.IsOpened()) {
			while (Window.PeekEvent(&Event)) {
				switch (Event.Type) {
				case Nt::Event::Types::KEY_DOWN:
					if (Event.Value != Nt::Keyboard::KEY_ESCAPE)
						break;
					[[fallthrough]];
				case Nt::Event::Types::WINDOW_CLOSE:
					Window.Destroy();
					break;
				}
			}
			Window.Clear();

			const float DeltaTime = Window.GetFrameTimeMs() / 1000.f;
			Cube.Rotate(RotateSpeed * DeltaTime);
			Cube.Render(&Window);

			Window.UpdateCamera();
			Window.Display();
		}
	}
	catch (const Nt::Error& NtError) {
		NtError.Show();
		Nt::Console::Pause();
	}
	return 0;
}