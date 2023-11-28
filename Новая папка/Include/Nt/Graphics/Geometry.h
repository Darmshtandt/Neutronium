#pragma once

namespace Nt {
	struct Vertex {
		constexpr Vertex() noexcept = default;
		constexpr Vertex(const Float3D& XYZ, const Float3D& nXnYnZ, const Float3D& UVW, const Float4D& RGBA) noexcept :
			Position(XYZ, 1.f),
			Normal(nXnYnZ, 1.f),
			TexCoord(UVW),
			Color(RGBA) {
		}

		Float4D Position;
		Float4D Normal;
		Float3D TexCoord;
		Float4D Color;
	};

	using Index_t = uShort;
	using Vertices_t = std::vector<Vertex>;
	using Indices_t = std::vector<Index_t>;

	struct Face {
		Float3D GetNormal() const noexcept {
			const Float3D edge1 = Vertices[2].Position - Vertices[1].Position;
			const Float3D edge2 = Vertices[3].Position - Vertices[1].Position;
			return edge1.GetCross(edge2);
		}

		Vertex Vertices[3];
		Index_t Indices[3];
	};

	struct Shape : public ISerialization {
		void Write(std::ostream& Stream) const {
			Serialization::WriteAll(Stream, Vertices, Indices);
		}
		void Read(std::istream& Stream) {
			Serialization::ReadAll(Stream, Vertices, Indices);
		}
		constexpr uInt Sizeof() const noexcept {
			return sizeof(*this);
		}
		constexpr uInt ClassType() const noexcept {
			return 0;
		}

		static ISerialization* New(const uInt& classType) {
			return new Shape;
		}

		Vertices_t Vertices;
		Indices_t Indices;
	};

	namespace Geometry {
		__inline constexpr Shape Quad(Float2D Size, const Float4D& Color, const FloatRect& TextureRect = { 0.f, 0.f, 1.f, 1.f }) {
			if (Size.Length() == 0.f) {
				Log::Warning("Quad is not created because size is 0");
				return { };
			}
			Size /= 2.f;

			Shape ShapeQuad;
			ShapeQuad.Vertices = {
				Vertex {
					Float3D(Size.x, -Size.y, 0.f),
					Float3D(0.f, 0.f, 0.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(-Size.x, Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(Size.x, Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Bottom),
					Color,
				},
			};
			ShapeQuad.Indices = {
				0, 3, 2,
				2, 1, 0
			};
			return ShapeQuad;
		}

		__inline constexpr Shape Cube(Float3D Size, const Float4D& Color, const FloatRect& TextureRect = { 0.f, 0.f, 1.f, 1.f }) {
			if (Size.Length() == 0.f) {
				Log::Warning("Quad is not created because size is 0");
				return { };
			}
			Size /= 2.f;

			Shape ShapeCube;
			ShapeCube.Vertices = {
				Vertex {
					Float3D(Size.x, -Size.y, -Size.z),
					Float3D(0.f, 0.f, 0.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(Size.x, Size.y, -Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(-Size.x, Size.y, -Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, -Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},

				Vertex {
					Float3D(Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, 0.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(Size.x, Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(-Size.x, Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},
			};

			for (uInt i = 0; i < 16; ++i) {
				Vertex NewVertex = ShapeCube.Vertices[i % 8];
				Float2D Coord = NewVertex.Position.yz;
				if (i >= 8)
					Coord = Float2D(NewVertex.Position.x, NewVertex.Position.z);

				NewVertex.TexCoord.x =
					(Coord.x <= 0) ? TextureRect.Left : TextureRect.Right;
				NewVertex.TexCoord.y =
					(Coord.y <= 0) ? TextureRect.Top : TextureRect.Bottom;

				if (i / 8 == 0)
					std::swap(NewVertex.TexCoord.x, NewVertex.TexCoord.y);

				ShapeCube.Vertices.push_back(NewVertex);
			}

			ShapeCube.Indices = {
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
			return ShapeCube;
		}

		__inline constexpr Shape Pyramid(Float3D Size, const Float4D& Color, const FloatRect& TextureRect = { 0.f, 0.f, 1.f, 1.f }) {
			if (Size.Length() == 0.f) {
				Log::Warning("Quad is not created because size is 0");
				return { };
			}
			Size /= 2.f;

			Shape ShapeCube;
			ShapeCube.Vertices = {
				Vertex {
					Float3D(Size.x, -Size.y, -Size.z),
					Float3D(0.f, 0.f, 0.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, -Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},

				Vertex {
					Float3D(0.f, Size.y, 0.f),
					Float3D(0.f, 0.f, 0.f),
					Float2D((TextureRect.Left + TextureRect.Right) / 2.f, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(0.f, Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D((TextureRect.Left + TextureRect.Right) / 2.f, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(0.f, Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D((TextureRect.Left + TextureRect.Right) / 2.f, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(0.f, Size.y, 0.f),
					Float3D(0.f, 0.f, -1.f),
					Float2D((TextureRect.Left + TextureRect.Right) / 2.f, TextureRect.Bottom),
					Color,
				},

				Vertex {
					Float3D(Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Top),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Top),
					Color,
				},

				Vertex {
					Float3D(Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Right, TextureRect.Bottom),
					Color,
				},
				Vertex {
					Float3D(-Size.x, -Size.y, Size.z),
					Float3D(0.f, 0.f, -1.f),
					Float2D(TextureRect.Left, TextureRect.Bottom),
					Color,
				},
			};

			ShapeCube.Indices = {
				0, 10, 11,
				11, 3, 0,

				0, 3, 4,
				2, 1, 5,

				6, 8, 0,
				7, 3, 9,
			};
			return ShapeCube;
		}
	}
}