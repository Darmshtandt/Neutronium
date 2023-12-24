#pragma once

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <CommCtrl.h>
#include <RichEdit.h>
#include <shlobj.h>

#include <KnownFolders.h>
#include <shobjidl.h>
#include <shlobj_core.h>
#include <shellapi.h>
#include <commoncontrols.h>

#include <Nt/Core.h>

// ============================================================================
//	Globals and classes
// ----------------------------------------------------------------------------
namespace Nt {
	class Window;
	class Renderer;
	class RenderWindow;
	class Shader;
	class IResource;
	class Image;
	class Texture;
	class VertexArray;
	class Mesh;
	class Model;
	class Sprite;
	class IObject;
	class Camera;
	class Camera2D;

#ifdef __NT__EXPEREMENTAL
	class GUI;
	class Font;
	class Text;
#endif
}


// ============================================================================
//	Neutronium
// ----------------------------------------------------------------------------
#include <Nt/Graphics/Geometry.h>
#include <Nt/Graphics/VertexArray.h>
#include <Nt/Graphics/IObject.h>
#include <Nt/Graphics/Shader.h>

#include <Nt/Graphics/Menu.h>
//#include <Nt/Graphics/HandleDeviceContext.h>
#include <Nt/Graphics/HandleWindow.h>
#include <Nt/Graphics/GDI/GDI.h>
#include <Nt/Graphics/GDI/Font.h>
#include <Nt/Graphics/GDI/Bitmap.h>
#include <Nt/Graphics/Icon.h>
#include <Nt/Graphics/WindowElements.h>
#include <Nt/Graphics/Window.h>
#include <Nt/Graphics/Layouts.h>

#include <Nt/Graphics/Resources/ResourceManager.h>
#include <Nt/Graphics/Resources/IResource.h>
#include <Nt/Graphics/Resources/Image.h>
#include <Nt/Graphics/Resources/Texture.h>
#include <Nt/Graphics/Resources/Mesh.h>

#include <Nt/Graphics/Renderer.h>
#include <Nt/Graphics/RenderWindow.h>
#include <Nt/Graphics/Model.h>
#include <Nt/Graphics/Sprite.h>
#include <Nt/Graphics/Camera.h>

#ifdef __NT__EXPEREMENTAL
#include <Nt/Graphics/GUI.h>

#include <Nt/Graphics/Font.h>
#include <Nt/Graphics/Text.h>
#endif

#pragma comment(lib, "Comdlg32")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "GLU32")

#ifdef _WIN64
#	pragma comment(lib, "glew64s")
#else
#	pragma comment(lib, "glew32s")
#endif