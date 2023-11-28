#pragma once

// ============================================================================
//	Globals and classes
// ----------------------------------------------------------------------------
namespace Nt {
	class Serialization;
	class String;
	class Timer;
	class JSONDict;
	class JSONLinkedList;
	class JSON;
	class Console;

	struct Error;
	struct ISerialization;
	struct Matrix3x3;
	struct Matrix4x4;
	struct JSONDataType;
	struct JSONContainer;
	struct JSONString;
	struct JSONInt;
	struct JSONFloat;
	struct JSONBool;
	struct Vertex;
	struct Face;
	struct Shape;

	template<class> class FlexibleList;
	template<typename> class SelfDeletingPtr;

	template<class> struct IFlexibleContainer;
	template<typename> struct Vector2D;
	template<typename> struct Vector3D;
	template<typename> struct Vector4D;
	template<typename> struct Rect;
}

#pragma warning(disable : 4201)
#pragma warning(disable : 4996)

// ============================================================================
//	STL
// ----------------------------------------------------------------------------
#include <fstream>
#include <codecvt>
#include <string>

#include <list>
#include <forward_list>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>


// ============================================================================
//	Neutronium
// ----------------------------------------------------------------------------
#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/SelfDeletingPtr.h>
#include <Nt/Core/Log.h>

#include <Nt/Core/IFlexibleContainer.h>
#include <Nt/Core/FlexibleList.h>
#include <Nt/Core/TableSet.h>

#include <Nt/Core/Math/Vectors.h>
#include <Nt/Core/Math/Rect.h>
#include <Nt/Core/Math/Matrix3x3.h>
#include <Nt/Core/Math/Matrix4x4.h>

#include <Nt/Core/Colors.h>
#include <Nt/Core/Serialization.h>
#include <Nt/Core/Timer.h>
#include <Nt/Core/Console.h>
#include <Nt/Core/Input.h>
#include <Nt/Core/JSON/DataTypes.h>
#include <Nt/Core/JSON/Dict.h>
#include <Nt/Core/JSON/LinkedList.h>
#include <Nt/Core/JSON/JSON.h>