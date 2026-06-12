#pragma once

#include <cassert>
#include <charconv>
#include <Nt/Core/Colors.h>
#include <Nt/Graphics/Geometry/Shape.h>

#include <fstream>
#include <mutex>
#include <unordered_map>

namespace Nt {
	struct OBJ final {
		struct VertexKey final {
			uInt v = -1;
			uInt vt = -1;
			uInt vn = -1;

			Bool operator == (const VertexKey& other) const noexcept {
				return v == other.v && vt == other.vt && vn == other.vn;
			}
		};

		struct VertexKeyHash {
			size_t operator()(const VertexKey& k) const {
				return ((size_t)k.v * 73856093) ^
					((size_t)k.vt * 19349663) ^
					((size_t)k.vn * 83492791);
			}
		};


		void ParseVertex(const std::string& line) noexcept {
			const Char* pLine = line.data() + 2;
			const Char* pEnd = line.data() + line.length();

			constexpr uInt maxArrSize = 7;
			Float scalars[maxArrSize] = { };
			uInt arraySize = 0;

			for (; arraySize < maxArrSize && pLine != pEnd; ++arraySize) {
				while (pLine < pEnd && isspace(*pLine))
					++pLine;

				auto charResult = std::from_chars(pLine, pEnd, scalars[arraySize]);
				if (charResult.ec != std::errc())
					return;
				pLine = charResult.ptr;
			}

			const Char type = line[1];
			switch (type) {
			case ' ':
				Positions.emplace_back(scalars[0], scalars[1], scalars[2]);
				if (arraySize == maxArrSize)
					Colors.emplace_back(scalars[3], scalars[4], scalars[5], scalars[6]);
				else if (arraySize > 3)
					Colors.emplace_back(scalars[3], scalars[4], scalars[5], 1.f);
				else
					Colors.emplace_back(1.f, 1.f, 1.f, 1.f);
				break;

			case 't':
				TexCoords.emplace_back(scalars[0], scalars[1], scalars[2]);
				break;

			case 'n':
				Normals.emplace_back(scalars[0], scalars[1], scalars[2]);
				break;

			default:
				puts("Unknown type");
				assert(0);
			}
		}

		void ParseFace(Shape& shape, const std::string& line) {
			const Char* pLine = line.data() + 2;
			const Char* pEnd = line.data() + line.length();

			for (uInt i = 0; i < 3; ++i) {
				VertexKey key = { };
				key.v = std::strtol(pLine, (char**)&pLine, 10) - 1;
				if (*pLine == '/' && *(++pLine) != '/')
					key.vt = std::strtol(pLine, (char**)&pLine, 10) - 1;
				if (*pLine == '/' && ++pLine != pEnd)
					key.vn = std::strtol(pLine, (char**)&pLine, 10) - 1;

				if (!VertexMap.contains(key)) {
					Vertex vert = { };
					vert.Position = Float4D(Positions[key.v], 1.f);
					vert.Color = Colors[key.v];

					if (key.vt != -1)
						vert.TexCoord = TexCoords[key.vt];
					if (key.vn != -1)
						vert.Normal = Float4D(Normals[key.vn], 1.f);

					const Index_t index = shape.Vertices.size();
					shape.Indices.emplace_back(index);
					shape.Vertices.emplace_back(vert);
					VertexMap[key] = index;
				}
				else {
					shape.Indices.emplace_back(VertexMap[key]);
				}
			}
		}

		NT_NODISCARD Shape LoadFromFile(const std::string& filePath) {
			std::ifstream file(filePath);
			if (!file.is_open())
				Raise("Failed to open: " + filePath);

			Shape result;
			std::string line;
			while (std::getline(file, line)) {
				if (line.empty())
					continue;

				switch (line.front()) {
				case 'v':
					ParseVertex(line);
					break;

				case 'f':
					ParseFace(result, line);
					break;

				case '#':
				case 'm':
				case 'o':
				case 's':
				case 'u':
					continue;

				default:
					Raise("Unknown type");
				}
			};

			file.close();

			return result;
		}

		std::vector<Float3D> Positions;
		std::vector<Float4D> Colors;
		std::vector<Float3D> TexCoords;
		std::vector<Float3D> Normals;
		std::unordered_map<VertexKey, uInt, VertexKeyHash> VertexMap;
	};
}