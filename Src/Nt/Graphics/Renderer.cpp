#include <Windows.h>
#include <shobjidl.h>

#include <GL\GLEW.h>
#include <GL\GL.h>

#include <functional>
#include <fstream>
#include <queue>

// ============================================================================
//	Neutronium
// ----------------------------------------------------------------------------
#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Colors.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/IObject.h>
#include <Nt/Graphics/Shader.h>

#include <Nt/Graphics/Menu.h>
#include <Nt/Graphics/HandleWindow.h>
#include <Nt/Graphics/Window.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Mesh.h>

#include <Nt/Graphics/Camera.h>
#include <Nt/Graphics/Renderer.h>


namespace Nt {
	void Renderer::CheckInitialization() const {
		if (!m_IsInitialized)
			Raise("Renderer is not initialized");
	}

	void Renderer::_Initialize(HWND hwnd) {
		if (wglGetCurrentContext() == nullptr) {
			m_hwnd = hwnd;

			PIXELFORMATDESCRIPTOR PFD = { };
			PFD.nSize = sizeof(PFD);
			PFD.nVersion = 1;
			PFD.cAlphaBits = 8;
			PFD.cAlphaShift = 8;
			PFD.cColorBits = 24;
			PFD.iLayerType = PFD_MAIN_PLANE;
			PFD.iPixelType = PFD_TYPE_RGBA;
			PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

			if (m_IsEnabled3D) {
				PFD.cDepthBits = 24;
				PFD.cStencilBits = 8;
			}

			const HDC hdc = GetDC(m_hwnd);
			const Int PixelFormat = ChoosePixelFormat(hdc, &PFD);

			if (PixelFormat == 0)
				Raise(L"Failed ChoosePixelFormat()");
			if (!SetPixelFormat(hdc, PixelFormat, &PFD))
				Raise(L"Failed SetPixelFormat()");
			if (DescribePixelFormat(hdc, PixelFormat, sizeof(PFD), &PFD) == 0)
				Raise(L"Failed DescribePixelFormat()");

			HGLRC hContext = wglCreateContext(hdc);
			wglMakeCurrent(hdc, hContext);

			glEnable(GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (m_IsEnabled3D)
				EnableDepthBuffer();

			m_Matrices.Projection.MakeIdentity();
			m_Matrices.World.MakeIdentity();
			m_Matrices.View.MakeIdentity();

			RECT ClientRect;
			GetWindowRect(m_hwnd, &ClientRect);
			Resize(uInt2D(ClientRect.right, ClientRect.bottom));

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
				Raise(L"Failed to initialize glew in DLL");

			m_IsInitialized = true;
		}
	}
}