#pragma once

#include <fstream>

#include <Nt/Core/Colors.h>
#include <Nt/Graphics/Geometry/Shape.h>

namespace Nt {
	struct OBJ {
		struct VertexData {
			Float4D Position = { 0.f, 0.f, 0.f, 1.f };
			Float3D Color;
		};


		Shape* LoadFromFile(const String& filePath) {
			FilePath = filePath;

			std::ifstream file(FilePath);
			if (!file.is_open())
				Raise("Failed to open: " + FilePath);

			LineNumber = 0;
			Shape* pShape;

			String line;
			while (std::getline(file, line)) {
				++LineNumber;

				if (line == "" || line == "\r")
					continue;

				std::vector<String> splitedString = line.Split(' ');
				auto getNextData = [&]() {
					if (splitedString.size() < 0)
						Raise(_GetErrorMessage());

					String& data = splitedString.front();
					splitedString.erase(splitedString.begin());
					return data;
					};

				const std::string type = splitedString[0];
				if (type == "#")
					continue;
				else if (type == "mtllib")
					_LoadMTL();
				else if (type == "usemtl")
					continue;

				Float3D position;
				Float3D color;

				if (type[0] != 'f') {
					const uInt parametersCount = splitedString.size() - 1;

					if (parametersCount == 0)
						Raise(_GetErrorMessage());

					for (uInt i = 0; i < parametersCount; ++i) {
						const String& parameter = splitedString[i + 1];

						if (!parameter.IsFloat())
							Raise(_GetErrorMessage());

						if (i < 3)
							position[i] = parameter;
						else
							color[i - 3] = parameter;
					}
				}

				switch (type[0]) {
				case 's':
				case 'g':
				case 'o':
				case '#':
					continue;

				case 'v':
					_LoadVertex(type, color, Float4D(position, 1.f));
					break;

				case 'f':
					_LoadFace(pShape, splitedString);
					break;

				default:
					Raise(_GetErrorMessage());
				}
			}
			file.close();

			return pShape;
		}

	private:
		String FilePath;
		std::vector<Float4D> Positions;
		std::vector<Float3D> Colors;
		std::vector<Float3D> TexCoords;
		std::vector<Float3D> Normals;
		uInt LineNumber = 0;

	private:
		String _GetErrorMessage() const {
			return "Failed to load mesh.\nFile name: " + FilePath + "\nLine: " + std::to_string(LineNumber);
		}

		void _LoadMTL() 
		{
		}

		void _LoadVertex(const std::string& type, const Float3D& color, const Float4D& position) {
			if (type == "v") {
				Colors.push_back(color);
				Positions.push_back(position);
			}
			else if (type == "vt") {
				TexCoords.push_back(Float3D(position));
			}
			else if (type == "vn") {
				Normals.push_back(position);
			}
			else if (type == "vp") {

			}
			else {
				Raise(_GetErrorMessage());
			}
		}
		void _LoadFace(Shape* pShape, std::vector<String>& splitedString) {
			for (uInt i = 1; i < splitedString.size(); ++i) {
				std::vector<String> splitedData = splitedString[i].Split('/');

				if (splitedData.size() <= 1)
					Raise(_GetErrorMessage());

				const Int vertexIndex = Int(splitedData[0]) - 1;

				Vertex vertex = { };
				vertex.Position = Positions[vertexIndex];
				vertex.Color =
					(!Colors.empty()) ? Float4D(Colors[vertexIndex], 1.f) : Colors::White;

				if (!splitedData[1].empty())
					vertex.TexCoord = TexCoords[Int(splitedData[1]) - 1];

				if (!splitedData[2].empty())
					vertex.Normal = Normals[Int(splitedData[2]) - 1];

				pShape->Vertices.push_back(vertex);
			}
		}
	};
}