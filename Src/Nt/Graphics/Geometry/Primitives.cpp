// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Log.h>
#include <Nt/Graphics/Geometry/Primitives.h>

namespace Nt::Primitive {
	NT_CONSTEXPR Shape Quad(Float2D size, const Float4D& color, const FloatRect& textureRect) {
		if (size.LengthSquare() == 0.f) {
			Log::Instance().Warning("Quad not created because size is 0");
			return { };
		}

		size /= 2.f;

		Shape shapeQuad;
		shapeQuad.Vertices = {
			Vertex {
				Float3D(size.x, -size.y, 0.f),
				Float3D(0.f, 0.f, 0.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(size.x, size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Bottom, 0.f),
				color,
			},
		};
		shapeQuad.Indices = {
			0, 3, 2,
			2, 1, 0
		};
		return shapeQuad;
	}

	NT_CONSTEXPR Shape Cube(Float3D size, const Float4D& color, const FloatRect& textureRect) {
		if (size.Length() == 0.f) {
			Log::Instance().Warning("Quad not created because size is 0");
			return { };
		}
		size /= 2.f;

		Shape shapeCube;
		shapeCube.Vertices = {
			Vertex {
				Float3D(size.x, -size.y, -size.z),
				Float3D(0.f, 0.f, 0.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(size.x, size.y, -size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, size.y, -size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, -size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},

			Vertex {
				Float3D(size.x, -size.y, size.z),
				Float3D(0.f, 0.f, 0.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(size.x, size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},
		};

		for (uInt i = 0; i < 16; ++i) {
			Vertex newVertex = shapeCube.Vertices[i % 8];
			Float2D position = newVertex.Position.yz;
			if (i >= 8)
				position = Float2D(newVertex.Position.x, newVertex.Position.z);

			newVertex.TexCoord.x =
				(position.x <= 0) ? textureRect.Left : textureRect.Right;
			newVertex.TexCoord.y =
				(position.y <= 0) ? textureRect.Top : textureRect.Bottom;

			if (i / 8 == 0)
				std::swap(newVertex.TexCoord.x, newVertex.TexCoord.y);

			shapeCube.Vertices.push_back(newVertex);
		}

		shapeCube.Indices = {
			0, 3, 2,
			2, 1, 0,

			4, 5, 6,
			6, 7, 4,

			5 + 8, 4 + 8, 0 + 8,
			0 + 8, 1 + 8, 5 + 8,

			7 + 8, 6 + 8, 2 + 8,
			2 + 8, 3 + 8, 7 + 8,

			0 + 16, 4 + 16, 7 + 16,
			7 + 16, 3 + 16, 0 + 16,

			1 + 16, 2 + 16, 6 + 16,
			6 + 16, 5 + 16, 1 + 16,
		};
		return shapeCube;
	}

	NT_CONSTEXPR Shape Pyramid(Float3D size, const Float4D& color, const FloatRect& textureRect) {
		if (size.Length() == 0.f) {
			Log::Instance().Warning("Quad not created because size is 0");
			return { };
		}
		size /= 2.f;

		Shape shapeCube;
		shapeCube.Vertices = {
			Vertex {
				Float3D(size.x, -size.y, -size.z),
				Float3D(0.f, 0.f, 0.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, -size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},

			Vertex {
				Float3D(0.f, size.y, 0.f),
				Float3D(0.f, 0.f, 0.f),
				Float3D((textureRect.Left + textureRect.Right) / 2.f, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(0.f, size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D((textureRect.Left + textureRect.Right) / 2.f, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(0.f, size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D((textureRect.Left + textureRect.Right) / 2.f, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(0.f, size.y, 0.f),
				Float3D(0.f, 0.f, -1.f),
				Float3D((textureRect.Left + textureRect.Right) / 2.f, textureRect.Bottom, 0.f),
				color,
			},

			Vertex {
				Float3D(size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Top, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Top, 0.f),
				color,
			},

			Vertex {
				Float3D(size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Right, textureRect.Bottom, 0.f),
				color,
			},
			Vertex {
				Float3D(-size.x, -size.y, size.z),
				Float3D(0.f, 0.f, -1.f),
				Float3D(textureRect.Left, textureRect.Bottom, 0.f),
				color,
			},
		};

		shapeCube.Indices = {
			0, 10, 11,
			11, 3, 0,

			0, 3, 4,
			2, 1, 5,

			6, 8, 0,
			7, 3, 9,
		};
		return shapeCube;
	}
	NT_CONSTEXPR Shape Line(Float length, const Float4D& color) {
		if (length == 0.f) {
			Log::Instance().Warning("Line not created because size is 0");
			return { };
		}
		length /= 2.f;

		Shape shapeCube;
		shapeCube.Vertices.emplace_back(Float3D(-length, 0.f, 0.f), Float3D(), Float3D(), color);
		shapeCube.Vertices.emplace_back(Float3D(length, 0.f, 0.f), Float3D(), Float3D(), color);

		shapeCube.Indices = { 0, 1 };
		return shapeCube;
	}
}