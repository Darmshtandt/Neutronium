#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <KnownFolders.h>
#include <shobjidl.h>
#include <shlobj_core.h>
#include <shellapi.h>
#include <commoncontrols.h>
#include <RichEdit.h>

#include <GL\GLEW.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <functional>
#include <fstream>
#include <queue>
#include <list>

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

#include <Nt/Core/Input.h>
#include <Nt/Core/Colors.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Timer.h>

#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/IObject.h>
#include <Nt/Graphics/Shader.h>

#include <Nt/Graphics/Menu.h>
#include <Nt/Graphics/HandleWindow.h>
#include <Nt/Graphics/GDI/GDI.h>
#include <Nt/Graphics/GDI/Bitmap.h>
#include <Nt/Graphics/Icon.h>
#include <Nt/Graphics/WindowElements.h>
#include <Nt/Graphics/Window.h>
#include <Nt/Graphics/Layouts.h>

#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Mesh.h>

#include <Nt/Graphics/Camera.h>
#include <Nt/Graphics/Renderer.h>
#include <Nt/Graphics/RenderWindow.h>


namespace Nt {
	void RenderWindow::Update() {
		if (m_CurrentCameraPtr) {
			m_CurrentCameraPtr->Update();
			Renderer::SetView(m_CurrentCameraPtr->GetView());
		}
	}
}